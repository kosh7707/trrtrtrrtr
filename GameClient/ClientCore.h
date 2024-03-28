#ifndef DATABASETERMPROJECTCLIENT_CLIENTCORE_H
#define DATABASETERMPROJECTCLIENT_CLIENTCORE_H

#include "ClientConstant.h"
#include "EventHandler.h"
#include <cstring>
#define BUF_SIZE 1024

class ClientCore {
public:
    ClientCore();
    [[noreturn]] void run();

private:
    ClientConstant      clientConstant;
    const std::string   serverIP;
    const int           serverPort;
    bool                isLogin;
    SOCKET              sc;
    EventHandler        eventHandler;
    std::queue<std::string> eventReqQueue;
    std::queue<std::string> eventResQueue;

    HANDLE  recvWorker;
    HANDLE  eventHandlingWorker;
    HANDLE  sendWorker;

    void    initClient();
    void    notifyServer(const std::string& msg);
    void    printUserCommand();

    void runRecvWorker();
    [[noreturn]] static unsigned int WINAPI runRecvWorkerThread(void* params);

    void runEventHandlingWorker();
    [[noreturn]] static unsigned int WINAPI runEventHandlingWorkerThread(void* params);

    void runSendWorker();
    [[noreturn]] static unsigned int WINAPI runSendWorkerThread(void* params);
};


#endif //DATABASETERMPROJECTCLIENT_CLIENTCORE_H
