#ifndef CHATSERVER_EVENTHANDLER_H
#define CHATSERVER_EVENTHANDLER_H


#include <iostream>
#include "IEventHandler.h"

class EventHandler final : public IEventHandler {
public:
    EventHandler() {}
    ~EventHandler() {}
    std::vector<std::unique_ptr<Event>> handling(std::unique_ptr<Event> event) override;
    std::vector<std::unique_ptr<Event>> userInputHandling(const std::string &command) override {
        return std::vector<std::unique_ptr<Event>>();
    }
private:
    std::vector<std::string> users;
};


#endif //CHATSERVER_EVENTHANDLER_H
