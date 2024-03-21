#ifndef DATABASETERMPROJECT_SERVERCORE_H
#define DATABASETERMPROJECT_SERVERCORE_H

#include <unordered_map>
#include "ServerConstant.h"
#include "Client.h"
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
    std::shared_ptr<EventHandler>           eventHandler;
    HANDLE                                  auctionWorker;
    AuctionDAO                              auctionDao;

    SOCKET  initServer();
    void runAuctionWorker();

    static unsigned int WINAPI runAuctionWorkerThread(void* params);

    void    addClient();
    void    readClient(const int index);
    void    removeClient(const int index);
    void    notifyClient(const int index, const std::string& msg);
    void    notifyAllClients(const std::string& msg);
};


#endif //DATABASETERMPROJECT_SERVERCORE_H
