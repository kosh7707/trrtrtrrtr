#ifndef DATABASETERMPROJECT_SERVERCORE_H
#define DATABASETERMPROJECT_SERVERCORE_H

#include "Client.h"
#include "EventHandler.h"
#include "ServerConstant.h"
#include "AuctionDAO.h"
#define BUF_SIZE 1024

class EventHandler;

class ServerCore {
    friend class EventHandler;
public:
    ServerCore();
    [[noreturn]] void run();
private:
    ServerConstant                          serverConstant;
    std::shared_ptr<Client[]>               Clients;
    std::string                             serverIP;
    int                                     maxClientCount;
    int                                     serverPort;
    int                                     ClientsCount;
    EventHandler                            eventHandler;
    AuctionDAO                              auctionDao;
    std::queue<std::pair<int, std::string>> eventReqQueue;
    std::queue<std::pair<int, std::string>> eventResQueue;

    HANDLE  auctionWorker;
    HANDLE  recvWorker;
    HANDLE  sendWorker;
    HANDLE  eventHandlingWorker;

    SOCKET  initServer();

    void runRecvWorker();
    [[noreturn]] static unsigned int WINAPI runRecvWorkerThread(void* params);

    void runEventHandlingWorker();
    [[noreturn]] static unsigned int WINAPI runEventHandlingWorkerThread(void* params);

    void runSendWorker();
    [[noreturn]] static unsigned int WINAPI runSendWorkerThread(void* params);

    void runAuctionWorker();
    [[noreturn]] static unsigned int WINAPI runAuctionWorkerThread(void* params);

    void    addClient();
    void    readClient(const int index);
    void    removeClient(const int index);
    void    notifyClient(const int index, const std::string& msg);
    void    notifyAllClients(const std::string& msg);
};


#endif //DATABASETERMPROJECT_SERVERCORE_H
