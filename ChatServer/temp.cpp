#include <iostream>
#include <thread>
#include <deque>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ChatClient {
public:
    ChatClient(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints, const std::string& channel, const std::string& map)
            : io_context_(io_context), socket_(io_context), channel_(channel), map_(map) {
        connect(endpoints);
    }

    void writeToChannel(const std::string& msg) {
        boost::asio::post(io_context_,
                          [this, msg]() {
                              bool write_in_progress = !write_msgs_.empty();
                              write_msgs_.push_back("channel\n" + msg + "\n");
                              if (!write_in_progress) {
                                  write();
                              }
                          });
    }

    void writeToMap(const std::string& msg) {
        boost::asio::post(io_context_,
                          [this, msg]() {
                              bool write_in_progress = !write_msgs_.empty();
                              write_msgs_.push_back("map\n" + msg + "\n");
                              if (!write_in_progress) {
                                  write();
                              }
                          });
    }

    void close() {
        boost::asio::post(io_context_, [this]() { socket_.close(); });
    }

private:
    void connect(const tcp::resolver::results_type& endpoints) {
        boost::asio::async_connect(socket_, endpoints,
                                   [this](boost::system::error_code ec, tcp::endpoint /*endpoint*/) {
                                       if (!ec) {
                                           // 채널과 맵 이름을 서버로 전송
                                           write(channel_ + "\n" + map_ + "\n");
                                           read();
                                       }
                                   });
    }

    void read() {
        boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(read_msg_), "\n",
                                      [this](boost::system::error_code ec, std::size_t length) {
                                          if (!ec) {
                                              std::cout << read_msg_.substr(0, length);
                                              read_msg_.erase(0, length);
                                              read();
                                          }
                                      });
    }

    void write() {
        boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front()),
                                 [this](boost::system::error_code ec, std::size_t /*length*/) {
                                     if (!ec) {
                                         write_msgs_.pop_front();
                                         if (!write_msgs_.empty()) {
                                             write();
                                         }
                                     } else {
                                         socket_.close();
                                     }
                                 });
    }

    void write(const std::string& msg) {
        boost::asio::async_write(socket_, boost::asio::buffer(msg),
                                 [this](boost::system::error_code ec, std::size_t /*length*/) {
                                     if (!ec) {
                                         // 메시지 전송 성공 후 아무것도 하지 않음
                                     } else {
                                         socket_.close();
                                     }
                                 });
    }

    boost::asio::io_context& io_context_;
    tcp::socket socket_;
    std::string read_msg_;
    std::deque<std::string> write_msgs_;
    std::string channel_;
    std::string map_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 5) {
            std::cerr << "Usage: chat_client <host> <port> <channel> <map>\n";
            return 1;
        }

        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        ChatClient client(io_context, endpoints, argv[3], argv[4]);

        std::thread t([&io_context]() { io_context.run(); });

        std::string line;
        while (std::getline(std::cin, line)) {
            if (line[0] == '/') {
                // 맵에 메시지 브로드캐스트
                client.writeToMap(line.substr(1) + "\n");
            } else {
                // 채널 전체에 메시지 브로드캐스트
                client.writeToChannel(line + "\n");
            }
        }

        client.close();
        t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}