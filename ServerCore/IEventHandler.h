#ifndef SERVERCORE_IEVENTHANDLER_H
#define SERVERCORE_IEVENTHANDLER_H


#include <vector>
#include <string>
#include "Event.h"

class IEventHandler {
public:
    virtual std::vector<Event> handling(Event event) = 0;
};


#endif //SERVERCORE_IEVENTHANDLER_H
