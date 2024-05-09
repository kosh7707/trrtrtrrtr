#ifndef CHATSERVER_CHANNEL_H
#define CHATSERVER_CHANNEL_H


#include <string>
#include <map>
#include "Map.h"

class Channel {
public:
    Channel() {}
    void join(SOCKET client, const std::string& map_name) {
        maps[map_name].join(client);
    }
    void leave(SOCKET client, const std::string& map_name) {
        maps[map_name].leave(client);
    }
    std::vector<SOCKET> channelBroadCast() {
        std::vector<SOCKET> ret;
        for (auto map : maps) {
            for (auto client : map.second.mapBroadCast())
                ret.emplace_back(client);
        }
        return ret;
    }
    std::vector<SOCKET> mapBroadCast(const std::string& map_name) {
        return maps[map_name].mapBroadCast();
    }
private:
    std::map<std::string, Map> maps;
};


#endif //CHATSERVER_CHANNEL_H
