#ifndef GAMECLIENT_EVENTHANDLER_H
#define GAMECLIENT_EVENTHANDLER_H


#include <iostream>
#include <sstream>
#include "IEventHandler.h"
#include "Constants/ItemConstant.h"
#define CHAT_SERVER_INDEX 0
#define MAIN_SERVER_INDEX 1

class EventHandler final : public IEventHandler {
public:
    EventHandler() {}
    std::vector<std::unique_ptr<Event>> handling(std::unique_ptr<Event> event) override;
    std::vector<std::unique_ptr<Event>> userInputHandling(const std::string& command) override;
private:
    int         chat_mode = 0;
    bool        isLogin = false;
    std::string user_id;
    std::string channel_name;
    std::string map_name;
    std::vector<std::string> split(const std::string& input, char delimeter = ' ') {
        std::vector<std::string> result;
        std::stringstream ss(input);
        std::string token;
        while (getline(ss, token, delimeter))
            result.push_back(token);
        return result;
    }
};


#endif //GAMECLIENT_EVENTHANDLER_H
