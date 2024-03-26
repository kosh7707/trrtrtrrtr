#ifndef DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H
#define DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H

#include <iostream>
#include <string>
#include <winsock2.h>
#include <process.h>
#include <vector>

using namespace std;

class ClientConstant {
public:
    const string& getServerIP() const {
        return serverIP;
    }
    const int getServerPort() const {
        return serverPort;
    }
private:
    const string  serverIP = "127.0.0.1";
    const int     serverPort = 7707;
};


#endif //DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H
