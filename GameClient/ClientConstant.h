#ifndef DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H
#define DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H

#include <iostream>
#include <string>
#include <winsock2.h>
#include <process.h>
#include <vector>
#include <queue>

enum {
    INVALID_EVENT = -1, LOGIN_EVENT, CHAT_EVENT
};

class ClientConstant {
public:
    const std::string& getServerIP() const {
        return serverIP;
    }
    const int getServerPort() const {
        return serverPort;
    }
private:
    const std::string   serverIP = "127.0.0.1";
    const int           serverPort = 7707;
};


#endif //DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H
