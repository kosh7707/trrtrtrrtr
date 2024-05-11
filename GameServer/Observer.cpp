#include "Observer.h"

void Observer::update(const int clientsCount, const std::unique_ptr<Socket[]>& connectedSockets) {
    std::lock_guard<std::mutex> lock(mutex);
    for (int i=0; i<clientsCount; i++) {
        int socket_id = connectedSockets[i].getSocketId();
        indexToSocketId.emplace(i, socket_id);
        if (socketIdToClient.find(socket_id) == socketIdToClient.end()) socketIdToClient[socket_id] = std::make_unique<Client>(i);
        else socketIdToClient[socket_id]->setIndex(i);
    }
}
