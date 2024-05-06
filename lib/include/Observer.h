#ifndef SERVERCORE_OBSERVER_H
#define SERVERCORE_OBSERVER_H

#include <memory>
#include "Socket.h"

class Observer {
public:
    virtual void update(const int clientsCount, std::unique_ptr<Socket[]>& connectedSockets) = 0;
    virtual ~Observer() {}
};


#endif //SERVERCORE_OBSERVER_H
