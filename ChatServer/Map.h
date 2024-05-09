#ifndef CHATSERVER_MAP_H
#define CHATSERVER_MAP_H


#include <memory>
#include <unordered_set>
#include <winsock2.h>
#include <vector>
#include "Client.h"

class Map {
public:
    Map() {}
    bool join(SOCKET client) {
        auto result = clients.insert(client);
        return result.second;
    }
    bool leave(SOCKET client) {
        return clients.erase(client) == 1;
    }
    std::vector<SOCKET> mapBroadCast() {
        std::vector<SOCKET> ret;
        for (const auto& it : clients)
            ret.emplace_back(it);
        return ret;
    }
private:
    std::unordered_set<SOCKET> clients;
};


#endif //CHATSERVER_MAP_H
