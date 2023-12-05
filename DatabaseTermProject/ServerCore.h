#ifndef DATABASETERMPROJECT_SERVERCORE_H
#define DATABASETERMPROJECT_SERVERCORE_H

#include <winsock2.h>
#include <memory>
#include <iostream>
#include <string>
#include <cstring>
#include <process.h>
#include "ServerConstant.h"
#include "Client.h"
#include "AccountDAO.h"
#define LOGIN_EVENT 0
#define CHAT_EVENT 1

using namespace std;

class ServerCore {
public:
    ServerCore();
    void run();
private:
    ServerConstant          serverConstant;
    AccountDAO              accountDao;
    shared_ptr<Client[]>    Clients;
    string                  serverIP;
    int                     maxClientCount;
    int                     serverPort;
    int                     ClientsCount;

    SOCKET  initServer();
    int     getEvent(const char* buf);
    string  getMessage(const char* buf);
    void    addClient();
    void    readClient(const int index);
    void    removeClient(const int index);
    void    notifyClient(const int index, const string& msg);
    void    notifyAllClients(const string& msg);
    void    handleLogin(const int index, const string& msg);
    void    handleChat(const int index, const string& msg);
};


#endif //DATABASETERMPROJECT_SERVERCORE_H
