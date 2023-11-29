#ifndef DATABASETERMPROJECT_SERVERCORE_H
#define DATABASETERMPROJECT_SERVERCORE_H

#include <winsock2.h>
#include <memory>
#include <iostream>
#include <string>
#include <cstring>
#include <process.h>
#include "ServerConstant.h"

using namespace std;

class ServerCore {
public:
    ServerCore();
    void run();
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

    SOCKET  initServer();
    void    addClient();
    void    readClient(const int index);
    void    removeClient(const int index);
    void    notifyClient(const int index, const string& msg);
    void    notifyAllClients(const string& msg);
    void    handleLogin(const int index, const string& msg);
};


#endif //DATABASETERMPROJECT_SERVERCORE_H
