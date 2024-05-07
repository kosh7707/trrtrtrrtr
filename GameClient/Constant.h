#ifndef DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H
#define DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H

#include <string>

class Constant {
public:
    const std::string getMainServerIP() const {
        return MainServerIP;
    }
    const int getMainServerPort() const {
        return MainServerPort;
    }
    const std::string getChatServerIp() const {
        return ChatServerIP;
    }
    const int getChatServerPort() const {
        return ChatServerPort;
    }

private:
    const std::string   MainServerIP   = "127.0.0.1";
    const int           MainServerPort = 7707;
    const std::string   ChatServerIP   = "127.0.0.1";
    const int           ChatServerPort = 7706;
};


#endif //DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H
