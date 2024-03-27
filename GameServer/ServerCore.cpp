#include "ServerCore.h"

ServerCore::ServerCore() :  maxClientCount(serverConstant.getMaxClientCount()),
                            serverPort(serverConstant.getServerPort()),
                            serverIP(serverConstant.getServerIP()) {
    Clients = std::shared_ptr<Client[]>(new Client[maxClientCount + 1]);
    ClientsCount = 0;
}

SOCKET ServerCore::initServer() {
    WSADATA wsadata;
    SOCKET sc;
    SOCKADDR_IN socketAddress;

    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsadata) != 0) {
        std::cout << "WSAStartup Error." << std::endl;
        return 0;
    }

    if ((sc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cout << "socket creation error." << std::endl;
        return 0;
    }

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    socketAddress.sin_port = htons(serverPort);
    if (bind(sc, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(socketAddress)) < 0) {
        std::cout << "bind error." << "\n";
        return 0;
    }

    if (listen(sc, SOMAXCONN) < 0) {
        std::cout << "listen error." << std::endl;
        return 0;
    }

    return sc;
}

[[noreturn]] void ServerCore::run() {
    SOCKET sc = initServer();
    if (sc == 0) {
        std::cout << "socket initialization error" << std::endl;
        exit(0);
    }

    HANDLE event = WSACreateEvent();
    Clients[ClientsCount].setEv(event);
    Clients[ClientsCount].setSc(sc);
    Clients[ClientsCount].setIp("0.0.0.0");

    WSAEventSelect(sc, event, FD_ACCEPT);
    ClientsCount++;

    runRecvWorker();
    runEventHandlingWorker();
    runSendWorker();
    runAuctionWorker();

    std::cout << "server listening at 127.0.0.1:7707" << std::endl;
    while (true);
}

void ServerCore::addClient() {
    SOCKADDR_IN socketAddress;
    SOCKET acceptSocket;
    int length;

    length = sizeof(socketAddress);
    acceptSocket = accept(Clients[0].getSc(), reinterpret_cast<SOCKADDR*>(&socketAddress), &length);

    HANDLE event = WSACreateEvent();
    Clients[ClientsCount].setEv(event);
    Clients[ClientsCount].setSc(acceptSocket);
    Clients[ClientsCount].setIp(inet_ntoa(socketAddress.sin_addr));

    WSAEventSelect(acceptSocket, event, FD_READ | FD_CLOSE);

    ClientsCount++;
}

void ServerCore::readClient(const int index) {
    char buf[BUF_SIZE];
    if (recv(Clients[index].getSc(), buf, BUF_SIZE, 0) <= 0)
        std::cerr << "recv error, errno: " << WSAGetLastError() << "\n";
    eventReqQueue.push({index, static_cast<std::string>(buf)});
}

void ServerCore::removeClient(const int index) {
    if (Clients[index].getAccount() == nullptr) {
        ClientsCount--;
        std::swap(Clients[index], Clients[ClientsCount]);
    }
    else {
        std::string removeClientIP = Clients[index].getIp();
        std::string removeClientNickName = Clients[index].getAccount()->getUserId();
        ClientsCount--;
        std::swap(Clients[index], Clients[ClientsCount]);
        std::string msg = "[1]Client Disconnected (IP: " + removeClientIP + ", name: " + removeClientNickName + ")";
        eventResQueue.push({0, msg});
    }
}

void ServerCore::notifyAllClients(const std::string& msg) {
    for (int i=1; i<ClientsCount; i++)
        if(send(Clients[i].getSc(), msg.c_str(), (int)strlen(msg.c_str()), 0) <= 0)
            std::cerr << "send error, errno: " << WSAGetLastError() << "\n";
}

void ServerCore::notifyClient(const int index, const std::string& msg) {
    if (send(Clients[index].getSc(), msg.c_str(), (int)strlen(msg.c_str()), 0) <= 0)
        std::cerr << "send error, errno: " << WSAGetLastError() << "\n";
}

void ServerCore::runRecvWorker() {
    unsigned int tid;
    recvWorker = (HANDLE)_beginthreadex(NULL, 0, runRecvWorkerThread, (void*)this, 0, &tid);
}

[[noreturn]] unsigned int WINAPI ServerCore::runRecvWorkerThread(void* params) {
    ServerCore* serverCore = static_cast<ServerCore*>(params);

    WSANETWORKEVENTS ev;
    WSAEVENT handleArray[serverCore->maxClientCount + 1];

    int index;
    while (true) {
        for (int i=0; i<serverCore->ClientsCount; i++)
            handleArray[i] = serverCore->Clients[i].getEv();
        index = WSAWaitForMultipleEvents(serverCore->ClientsCount, handleArray, false, INFINITE, false);
        if ((index != WSA_WAIT_FAILED) and (index != WSA_WAIT_TIMEOUT)) {
            WSAEnumNetworkEvents(serverCore->Clients[index].getSc(), serverCore->Clients[index].getEv(), &ev);
            if (ev.lNetworkEvents == FD_ACCEPT) serverCore->addClient();
            else if (ev.lNetworkEvents == FD_READ) serverCore->readClient(index);
            else if (ev.lNetworkEvents == FD_CLOSE) serverCore->removeClient(index);
        }
    }
}

void ServerCore::runEventHandlingWorker() {
    unsigned int tid;
    eventHandlingWorker = (HANDLE)_beginthreadex(NULL, 0, runEventHandlingWorkerThread, (void*)this, 0, &tid);
}

[[noreturn]] unsigned int WINAPI ServerCore::runEventHandlingWorkerThread(void* params) {
    ServerCore* serverCore = static_cast<ServerCore*>(params);

    while (true) {
        if (serverCore->eventReqQueue.empty()) continue;
        auto event = serverCore->eventReqQueue.front(); serverCore->eventReqQueue.pop();
        auto res = serverCore->eventHandler.handling(event.first, event.second, serverCore->ClientsCount, serverCore->Clients);
        for (auto it : res)
            serverCore->eventResQueue.push(it);
    }
}

void ServerCore::runSendWorker() {
    unsigned int tid;
    sendWorker = (HANDLE)_beginthreadex(NULL, 0, runSendWorkerThread, (void*)this, 0, &tid);
}

[[noreturn]] unsigned int WINAPI ServerCore::runSendWorkerThread(void* params) {
    ServerCore* serverCore = static_cast<ServerCore*>(params);

    while (true) {
        if (serverCore->eventResQueue.empty()) continue;
        auto event = serverCore->eventResQueue.front(); serverCore->eventResQueue.pop();
        if (event.first == 0) serverCore->notifyAllClients(event.second);
        else serverCore->notifyClient(event.first, event.second);
    }
}

void ServerCore::runAuctionWorker() {
    unsigned int tid;
    auctionWorker = (HANDLE)_beginthreadex(NULL, 0, runAuctionWorkerThread, (void*)this, 0, &tid);
}

[[noreturn]] unsigned int WINAPI ServerCore::runAuctionWorkerThread(void* params) {
    ServerCore* serverCore = static_cast<ServerCore*>(params);
    while (true) {
        auto res = serverCore->auctionDao.outdatedItemCheck(serverCore->ClientsCount, serverCore->Clients);
        for (auto it : res)
            serverCore->eventResQueue.push(it);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}


