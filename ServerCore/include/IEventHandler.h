#ifndef SERVERCORE_IEVENTHANDLER_H
#define SERVERCORE_IEVENTHANDLER_H


#include <vector>
#include <string>
#include <memory>
#include "Event.h"
#include "Queue.h"

class IEventHandler {
public:
    virtual std::vector<std::unique_ptr<Event>> handling(std::unique_ptr<Event> event) = 0;
    virtual std::vector<std::unique_ptr<Event>> userInputHandling(const std::string& command) = 0;
    virtual ~IEventHandler() {}

    // TODO: 경매장 서버를 구현하면 삭제할 예정
    Queue<Event> eventReqQueue;
};


#endif //SERVERCORE_IEVENTHANDLER_H
