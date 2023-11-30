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
    void    addClient();
    void    readClient(const int index);
    void    removeClient(const int index);
    void    notifyClient(const int index, const string& msg);
    void    notifyAllClients(const string& msg);
    void    handleLogin(const int index, const string& msg);
};


#endif //DATABASETERMPROJECT_SERVERCORE_H
