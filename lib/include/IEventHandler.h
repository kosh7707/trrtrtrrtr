#ifndef SERVERCORE_IEVENTHANDLER_H
#define SERVERCORE_IEVENTHANDLER_H


#include <vector>
#include <string>
#include <memory>
#include "Event.h"

class IEventHandler {
public:
    virtual std::vector<std::unique_ptr<Event>> handling(std::unique_ptr<Event> event) = 0;
};


#endif //SERVERCORE_IEVENTHANDLER_H
