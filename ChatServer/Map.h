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
    void join(SOCKET client) {
        clients.insert(client);
    }
    void leave(SOCKET client) {
        clients.erase(client);
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
