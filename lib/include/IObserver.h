#ifndef SERVERCORE_IOBSERVER_H
#define SERVERCORE_IOBSERVER_H

#include <memory>
#include "Socket.h"

class IObserver {
public:
    virtual void update(const int clientsCount, const std::unique_ptr<Socket[]>& connectedSockets) = 0;
    virtual ~IObserver() {}
};


#endif //SERVERCORE_IOBSERVER_H
