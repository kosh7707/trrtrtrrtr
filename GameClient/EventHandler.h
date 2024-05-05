#ifndef GAMECLIENT_EVENTHANDLER_H
#define GAMECLIENT_EVENTHANDLER_H


#include <iostream>
#include <sstream>
#include "IEventHandler.h"
#define MAIN_SERVER_INDEX 0

class EventHandler final : public IEventHandler {
public:
    EventHandler() {}
    std::vector<std::unique_ptr<Event>> handling(std::unique_ptr<Event> event) override;
    std::vector<std::unique_ptr<Event>> userInputHandling(const std::string& command) override;
private:
    bool isLogin = false;

    std::vector<std::string> split(const std::string& input);
    void printUserCommand();
};


#endif //GAMECLIENT_EVENTHANDLER_H
