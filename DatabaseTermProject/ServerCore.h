#ifndef DATABASETERMPROJECT_SERVERCORE_H
#define DATABASETERMPROJECT_SERVERCORE_H

#include <winsock2.h>
#include <thread>
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
#define SELL_ITEM_EVENT 4
#define BUY_NOW_EVENT 5
#define BID_EVENT 6
#define BREAK_ITEM_EVENT 7
#define OPEN_PERMISSION_STORE_EVENT 8
#define BUY_PERMISSION_EVENT 9

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
    HANDLE                  auctionWorker;

    SOCKET  initServer();
    void runAuctionWorker();

    static unsigned int WINAPI runAuctionWorkerThread(void* params);

    int     getEvent(const char* buf);
    string  getMessage(const char* buf);
    void    addClient();
    void    readClient(const int index);
    void    removeClient(const int index);
    void    notifyClient(const int index, const string& msg);
    void    notifyAllClients(const string& msg);
    vector<string> split(const string& input, char delimiter);
    void    handleLogin(const int index, const string& msg);
    void    handleChat(const int index, const string& msg);
    void    handleGetTestItem(const int index);
    void    handleInventoryCheck(const int index);
    void    handleSellItem(const int index, const string& msg);
    void    handleBuyNow(const int index, const string& msg);
    void    handleBid(const int index, const string& msg);
    void    handleBreakItem(const int index, const string& msg);
    void    handleOpenPermissionStore(const int index);
    void    handleBuyPermission(const int index, const string& msg);
};


#endif //DATABASETERMPROJECT_SERVERCORE_H
