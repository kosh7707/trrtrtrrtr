#include "EventHandler.h"

std::vector<std::string> EventHandler::serverEventHandling(bool isLogin, const std::string& buf) {
    if (!isLogin) {

    }
    else {

    }
}

std::string EventHandler::userEventHandling(bool isLogin, const std::string& buf) {
    std::string ret;
    if (!isLogin) {
        std::vector<std::string> command = split(buf, ' ');
        if (command[0] == "!login") ret = "[0]" + command[1] + "," + command[2];
    }
    else {
        if (buf[0] != '!') ret ="[1]" + buf;
        else {
            std::vector<std::string> command = split(buf, ' ');
            if (command[0] == "getTestItem") ret = "[2]";
            else if (command[0] == "inventoryCheck") ret = "[3]";
            else if (command[0] == "sellItem") ret = "[4]" + command[1] + "," + command[2] + "," + command[3] + "," + command[4];
            else if (command[0] == "buyNow") ret = "[5]" + command[1];
            else if (command[0] == "bid") ret = "[6]" + command[1] + "," + command[2];
            else if (command[0] == "breakItem") ret = "[7]" + command[1] + "," + command[2];
            else if (command[0] == "auctionCheck") ret = "[8]";
        }
    }
    return ret;
}