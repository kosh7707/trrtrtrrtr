#ifndef CHATSERVER_CHANNELMANAGER_H
#define CHATSERVER_CHANNELMANAGER_H


#include "Channel.h"

class ChannelManager {
public:
    ChannelManager() {}
    void join(SOCKET client, const std::string& channel_name, const std::string& map_name) {
        channels[channel_name].join(client, map_name);
    }
    void leave(SOCKET client, const std::string& channel_name, const std::string& map_name) {
        channels[channel_name].leave(client, map_name);
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
