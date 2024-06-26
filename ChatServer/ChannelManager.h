#ifndef CHATSERVER_CHANNELMANAGER_H
#define CHATSERVER_CHANNELMANAGER_H


#include "Channel.h"

class ChannelManager {
public:
    ChannelManager() {}
    bool join(SOCKET client, const std::string& channel_name, const std::string& map_name) {
        return channels[channel_name].join(client, map_name);
    }
    bool leave(SOCKET client, const std::string& channel_name, const std::string& map_name) {
        return channels[channel_name].leave(client, map_name);
    }
    std::vector<SOCKET> BroadCast() {
        std::vector<SOCKET> ret;
        for (auto channel : channels) {
            for (auto client : channel.second.channelBroadCast())
                ret.emplace_back(client);
        }
        return ret;
    }
    std::vector<SOCKET> channelBroadCast(const std::string& channel_name) {
        return channels[channel_name].channelBroadCast();
    }
    std::vector<SOCKET> mapBroadCast(const std::string& channel_name, const std::string& map_name) {
        return channels[channel_name].mapBroadCast(map_name);
    }
private:
    std::map<std::string, Channel> channels;
};


#endif //CHATSERVER_CHANNELMANAGER_H
