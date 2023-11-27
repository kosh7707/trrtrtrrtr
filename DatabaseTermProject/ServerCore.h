#ifndef DATABASETERMPROJECT_SERVERCORE_H
#define DATABASETERMPROJECT_SERVERCORE_H


#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <memory>
#include "ServerConstant.h"

using namespace std;

class ServerCore {
public:
    ServerCore();
    int run();
private:
    typedef struct {
        SOCKET sc;
        HANDLE ev;
        string nickname;
        string clientIP;
    } SOCK_INFO;
    ServerConstant          serverConstant;
    const int               maxClientCount;
    const int               serverPort;
    const string            serverIP;
    shared_ptr<SOCK_INFO[]> socketArray;
    int                     socketCount;

    SOCKET initServer();

};


#endif //DATABASETERMPROJECT_SERVERCORE_H
