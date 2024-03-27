#ifndef GAMECLIENT_EVENTHANDLER_H
#define GAMECLIENT_EVENTHANDLER_H

#include <vector>
#include <string>
#include <sstream>

class EventHandler {
public:
    std::vector<std::string> serverEventHandling(bool isLogin, const std::string& buf);
    std::string userEventHandling(bool isLogin, const std::string& buf);
private:
    std::vector<std::string> split(const std::string& input, char delimiter) {
        std::vector<std::string> result;
        std::stringstream ss(input);
        std::string token;
        while (getline(ss, token, delimiter))
            result.push_back(token);
        return result;
    }
    enum {
        INVALID_EVENT = -1, LOGIN_EVENT, CHAT_EVENT
    };
};


#endif //GAMECLIENT_EVENTHANDLER_H
