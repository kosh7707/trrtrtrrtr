#ifndef GAMESERVER_OBSERVER_H
#define GAMESERVER_OBSERVER_H


#include <iostream>
#include <unordered_map>
#include "IObserver.h"
#include "Client.h"

class Observer final : public IObserver {
    friend class EventHandler;
public:
    void update(const int clientsCount, const std::unique_ptr<Socket[]>& connectedSockets) override;
    ~Observer() {}
private:
    std::unordered_map<int, Socket>     indexToSocket;
    std::unordered_map<SOCKET, Client>  socketToClient;
};


#endif //GAMESERVER_OBSERVER_H
