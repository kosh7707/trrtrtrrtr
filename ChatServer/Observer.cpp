#include "Observer.h"

void Observer::update(const int clientsCount, const std::unique_ptr<Socket[]>& connectedSockets) {
    std::lock_guard<std::mutex> lock(mutex);
    for (int i=0; i<clientsCount; i++) {
        SOCKET sc = connectedSockets[i].getSc();
        indexToSocket.emplace(i, sc);
        socketToIndex.emplace(sc, i);
    }
}
