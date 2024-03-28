#include "EventHandler.h"

int EventHandler::getEvent(const std::string& buf) {
    std::string prefix = buf.substr(0, 3);
    if (prefix == "[0]") return LOGIN_EVENT;
    else if (prefix == "[1]") return CHAT_EVENT;
    else return INVALID_EVENT;
}

std::string EventHandler::getMessage(const std::string& buf) {
    std::string temp = buf;
    return temp.substr(3);
}

std::vector<std::string> EventHandler::split(const std::string& input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;
    while (getline(ss, token, ' '))
        result.push_back(token);
    return result;
}

std::pair<int, std::string> EventHandler::serverEventHandling(bool isLogin, const std::string& buf) {
    std::pair<int, std::string> ret;
    int event = getEvent(buf); std::string msg = getMessage(buf);
    if (!isLogin) {
        if (event == LOGIN_EVENT)
            ret = {LOGIN_EVENT, msg};
    }
    else {
        if (event == CHAT_EVENT)
            ret = {CHAT_EVENT, msg};
    }
    return ret;
}

std::string EventHandler::userEventHandling(bool isLogin, const std::string& buf) {
    std::string ret;
    try {
        if (!isLogin) {
            std::vector<std::string> command = split(buf);
            if (command[0] == "!login") ret = "[0]" + command[1] + "," + command[2];
        } else {
            if (buf[0] != '!') ret = "[1]" + buf;
            else {
                std::vector<std::string> command = split(buf);
                if (command[0] == "!getTestItem") ret = "[2]";
                else if (command[0] == "!inventoryCheck") ret = "[3]";
                else if (command[0] == "!sellItem") ret = "[4]" + command[1] + "," + command[2] + "," + command[3] + "," + command[4];
                else if (command[0] == "!buyNow") ret = "[5]" + command[1];
                else if (command[0] == "!bid") ret = "[6]" + command[1] + "," + command[2];
                else if (command[0] == "!breakItem") ret = "[7]" + command[1] + "," + command[2];
                else if (command[0] == "!auctionCheck") ret = "[8]";
            }
        }
    } catch (std::bad_alloc e) {
        std::cout << "please enter correct command\n";
    }
    return ret;
}