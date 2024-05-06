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
    std::vector<std::string> split(const std::string& input) {
        std::vector<std::string> result;
        std::stringstream ss(input);
        std::string token;
        while (getline(ss, token, ' '))
            result.push_back(token);
        return result;
    }
};


#endif //GAMECLIENT_EVENTHANDLER_H
