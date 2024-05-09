#ifndef CHATSERVER_OBSERVER_H
#define CHATSERVER_OBSERVER_H


#include <iostream>
#include <unordered_map>
#include <mutex>
#include "IObserver.h"
#include "Client.h"

class Observer final : public IObserver {
    friend class EventHandler;
public:
    void update(const int clientsCount, const std::unique_ptr<Socket[]>& connectedSockets) override;
    ~Observer() {}
private:
    std::unordered_map<int, SOCKET>  indexToSocket;
    std::unordered_map<SOCKET, int>  socketToIndex;
    std::mutex mutex;
};



#endif //CHATSERVER_OBSERVER_H
