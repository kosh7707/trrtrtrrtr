#ifndef SERVERCORE_SERVERCORE_H
#define SERVERCORE_SERVERCORE_H

#include <string>
#include <winsock2.h>
#include <thread>
#include <memory>
#include <iostream>
#include <string>
#include <cstring>
#include <process.h>
#include <vector>
#include <queue>
#include <unistd.h>
#include "IEventHandler.h"
#include "Socket.h"
#include "Event.h"
#include "Queue.h"
#include "IObserver.h"

#define BUF_SIZE 1024

class ServerCore {
public:
    ServerCore(bool isServer, std::unique_ptr<IEventHandler> eventHandler, std::string serverIP, int serverPort);
    void attachSocketObserver(std::shared_ptr<IObserver> observer);
    bool connect(const std::string& destServerIP, const int destServerPort);
    [[noreturn]] void run();
private:
    bool            isServer;
    std::string     serverIP;
    int             serverPort;
    int             clientsCount;
    Socket          sc;

    std::vector<std::shared_ptr<IObserver>> observers;
    std::unique_ptr<Socket[]>               connectedSockets;

    Queue<Event>                    eventReqQueue;
    Queue<Event>                    eventResQueue;
    std::unique_ptr<IEventHandler>  eventHandler;

    HANDLE  recvWorker;
    HANDLE  eventHandlingWorker;
    HANDLE  sendWorker;


    // TODO: 경매장 서버를 구현하면 삭제할 예정
    HANDLE tempWorker;
    void runTempWorker();
    [[noreturn]] static unsigned int WINAPI runTempWorkerThread(void* params);


    bool initServer();

    bool accept();

    bool close(const int index);

    bool recv(const int index);

    bool send(std::unique_ptr<Event> event);

    void runRecvWorker();
    [[noreturn]] static unsigned int WINAPI runRecvWorkerThread(void* params);

    void runEventHandlingWorker();
    [[noreturn]] static unsigned int WINAPI runEventHandlingWorkerThread(void* params);

    void runSendWorker();
    [[noreturn]] static unsigned int WINAPI runSendWorkerThread(void* params);

    void publish();
};


#endif //SERVERCORE_SERVERCORE_H
