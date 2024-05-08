#include <iostream>
#include <map>
#include <unordered_set>
#include <memory>
#include <string>
#include <deque>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ChatParticipant {
public:
    virtual ~ChatParticipant() = default;
    virtual void deliver(const std::string& msg) = 0;
};

using ChatParticipantPtr = std::shared_ptr<ChatParticipant>;

class Map {
public:
    void join(ChatParticipantPtr participant) {
        participants_.insert(participant);
    }

    void leave(ChatParticipantPtr participant) {
        participants_.erase(participant);
    }

    void deliver(const std::string& msg) {
        for (auto& participant : participants_) {
            participant->deliver(msg);
        }
    }

private:
    std::unordered_set<ChatParticipantPtr> participants_;
};

class Channel {
public:
    void joinMap(const std::string& map_name, ChatParticipantPtr participant) {
        maps_[map_name].join(participant);
    }

    void leaveMap(const std::string& map_name, ChatParticipantPtr participant) {
        if (maps_.count(map_name)) {
            maps_[map_name].leave(participant);
        }
    }

    void deliverToMap(const std::string& map_name, const std::string& msg) {
        if (maps_.count(map_name)) {
            maps_[map_name].deliver(msg);
        }
    }

    void deliverToChannel(const std::string& msg) {
        for (auto& [map_name, map] : maps_) {
            map.deliver(msg);
        }
    }

private:
    std::map<std::string, Map> maps_;
};

class ChatRoom {
public:
    void joinChannel(const std::string& channel_name, const std::string& map_name, ChatParticipantPtr participant) {
        channels_[channel_name].joinMap(map_name, participant);
    }

    void leaveChannel(const std::string& channel_name, const std::string& map_name, ChatParticipantPtr participant) {
        if (channels_.count(channel_name)) {
            channels_[channel_name].leaveMap(map_name, participant);
        }
    }

    void deliverToChannel(const std::string& channel_name, const std::string& msg) {
        if (channels_.count(channel_name)) {
            channels_[channel_name].deliverToChannel(msg);
        }
    }

    void deliverToMap(const std::string& channel_name, const std::string& map_name, const std::string& msg) {
        if (channels_.count(channel_name)) {
            channels_[channel_name].deliverToMap(map_name, msg);
        }
    }

private:
    std::map<std::string, Channel> channels_;
};

class ChatSession : public ChatParticipant, public std::enable_shared_from_this<ChatSession> {
public:
    ChatSession(tcp::socket socket, ChatRoom& room)
            : socket_(std::move(socket)), room_(room) {}

    void start() {
        readChannelAndMap();
    }

    void deliver(const std::string& msg) override {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress) {
            write();
        }
    }

private:
    void readChannelAndMap() {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(read_msg_), "\n",
                                      [this, self](boost::system::error_code ec, std::size_t length) {
                                          if (!ec) {
                                              std::istringstream iss(read_msg_.substr(0, length - 1));
                                              read_msg_.erase(0, length);
                                              std::getline(iss, channel_);
                                              std::getline(iss, map_);
                                              room_.joinChannel(channel_, map_, shared_from_this());
                                              readMessageType();
                                          } else {
                                              socket_.close();
                                          }
                                      });
    }

    void readMessageType() {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(read_msg_), "\n",
                                      [this, self](boost::system::error_code ec, std::size_t length) {
                                          if (!ec) {
                                              std::string type = read_msg_.substr(0, length - 1); // Remove newline
                                              read_msg_.erase(0, length);
                                              if (type == "channel") {
                                                  readAndDeliverToChannel();
                                              } else if (type == "map") {
                                                  readAndDeliverToMap();
                                              } else {
                                                  readMessageType(); // Continue reading message type
                                              }
                                          } else {
                                              room_.leaveChannel(channel_, map_, shared_from_this());
                                          }
                                      });
    }

    void readAndDeliverToChannel() {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(read_msg_), "\n",
                                      [this, self](boost::system::error_code ec, std::size_t length) {
                                          if (!ec) {
                                              std::string msg = read_msg_.substr(0, length);
                                              read_msg_.erase(0, length);
                                              room_.deliverToChannel(channel_, msg);
                                              readMessageType();
                                          } else {
                                              room_.leaveChannel(channel_, map_, shared_from_this());
                                          }
                                      });
    }

    void readAndDeliverToMap() {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(read_msg_), "\n",
                                      [this, self](boost::system::error_code ec, std::size_t length) {
                                          if (!ec) {
                                              std::string msg = read_msg_.substr(0, length);
                                              read_msg_.erase(0, length);
                                              room_.deliverToMap(channel_, map_, msg);
                                              readMessageType();
                                          } else {
                                              room_.leaveChannel(channel_, map_, shared_from_this());
                                          }
                                      });
    }

    void write() {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front()),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                     if (!ec) {
                                         write_msgs_.pop_front();
                                         if (!write_msgs_.empty()) {
                                             write();
                                         }
                                     } else {
                                         room_.leaveChannel(channel_, map_, shared_from_this());
                                     }
                                 });
    }

    tcp::socket socket_;
    ChatRoom& room_;
    std::string channel_;
    std::string map_;
    std::string read_msg_;
    std::deque<std::string> write_msgs_;
};

class ChatServer {
public:
    ChatServer(boost::asio::io_context& io_context, const tcp::endpoint& endpoint)
            : acceptor_(io_context, endpoint) {
        accept();
    }

private:
    void accept() {
        acceptor_.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket) {
                    if (!ec) {
                        std::make_shared<ChatSession>(std::move(socket), room_)->start();
                    }
                    accept();
                });
    }

    tcp::acceptor acceptor_;
    ChatRoom room_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::endpoint endpoint(tcp::v4(), 15001);
        ChatServer server(io_context, endpoint);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}