#ifndef DATABASETERMPROJECT_SERVERCORE_H
#define DATABASETERMPROJECT_SERVERCORE_H

#include <winsock2.h>
#include <memory>
#include <iostream>
#include <string>
#include <cstring>
#include <process.h>
#include <vector>
#include "ServerConstant.h"
#include "Client.h"
#include "AccountDAO.h"
#include "InventoryDAO.h"
#define INVALID_EVENT -1
#define LOGIN_EVENT 0
#define CHAT_EVENT 1
#define GET_TEST_ITEM_EVENT 2
#define INVENTORY_CHECK_EVENT 3

using namespace std;

class ServerCore {
public:
    ServerCore();
    [[noreturn]] void run();
private:
    ServerConstant          serverConstant;
    AccountDAO              accountDao;
    InventoryDAO            inventoryDao;
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
    void    handleGetTestItem(const int index);
    void    handleInventoryCheck(const int index);
};


#endif //DATABASETERMPROJECT_SERVERCORE_H
