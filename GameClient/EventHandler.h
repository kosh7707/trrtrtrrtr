#ifndef GAMECLIENT_EVENTHANDLER_H
#define GAMECLIENT_EVENTHANDLER_H

#include <vector>
#include <string>
#include <sstream>
#include "ClientConstant.h"

class EventHandler {
public:
    std::pair<int, std::string> serverEventHandling(bool isLogin, const std::string& buf);
    std::string userEventHandling(bool isLogin, const std::string& buf);
private:
    int getEvent(const std::string& buf);
    std::string getMessage(const std::string& buf);
    std::vector<std::string> split(const std::string& input);
};


#endif //GAMECLIENT_EVENTHANDLER_H
