#include "Observer.h"

void Observer::update(const int clientsCount, const std::unique_ptr<Socket[]>& connectedSockets) {
    std::lock_guard<std::mutex> lock(mutex);
    for (int i=0; i<clientsCount; i++) {
        indexToSocket.emplace(i, connectedSockets[i]);
        SOCKET sc = connectedSockets[i].getSc();
        if (socketToClient.find(sc) != socketToClient.end()) socketToClient[sc].setIndex(i);
        else socketToClient[sc] = Client(i);
    }
}
