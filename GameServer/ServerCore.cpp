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

/*-------------------
| Client Handling Part
-------------------*/

[[noreturn]] void ServerCore::run() {
    runAuctionWorker();

    WSANETWORKEVENTS ev;
    WSAEVENT handleArray[maxClientCount + 1];

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

    std::cout << "server listening at 127.0.0.1:7707" << std::endl;
    int index;
    while (true) {
        for (int i=0; i<ClientsCount; i++)
            handleArray[i] = Clients[i].getEv();
        index = WSAWaitForMultipleEvents(ClientsCount, handleArray, false, INFINITE, false);
        if ((index != WSA_WAIT_FAILED) and (index != WSA_WAIT_TIMEOUT)) {
            WSAEnumNetworkEvents(Clients[index].getSc(), Clients[index].getEv(), &ev);
            if (ev.lNetworkEvents == FD_ACCEPT) addClient();
            else if (ev.lNetworkEvents == FD_READ) readClient(index);
            else if (ev.lNetworkEvents == FD_CLOSE) removeClient(index);
        }
    }
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
    char buf[BUF_SIZE + 1];
    recv(Clients[index].getSc(), buf, BUF_SIZE, 0);
    std::cout << "[Recv] Clients[" << index << "]: " << buf << "\n";
    auto res = eventHandler.handling(index, buf, ClientsCount, Clients);
    for (auto it : res) {
        if (it.first == 0) notifyAllClients(it.second);
        else notifyClient(it.first, it.second);
    }
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
        notifyAllClients("[1]Client Disconnected (IP: " + removeClientIP + ", name: " + removeClientNickName + ")");
    }
}

void ServerCore::notifyAllClients(const std::string& msg) {
    std::cout << "[send, all clients]: " << msg << "\n";
    for (int i=1; i<ClientsCount; i++)
        send(Clients[i].getSc(), msg.c_str(), BUF_SIZE, 0);
}

void ServerCore::notifyClient(const int index, const std::string& msg) {
    std::cout << "[send, client[" << index << "]]: " << msg << "\n";
    send(Clients[index].getSc(), msg.c_str(), BUF_SIZE, 0);
}

unsigned int WINAPI ServerCore::runAuctionWorkerThread(void* params) {
    ServerCore* serverCore = static_cast<ServerCore*>(params);
    IDatabaseConnection& dc = DatabaseConnection::getInstance();
    while (dc.isConnected()) {
        auto res = serverCore->auctionDao.outdatedItemCheck(serverCore->ClientsCount, serverCore->Clients);
        for (const auto& it : res)
            serverCore->notifyClient(it.first, it.second);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return 0;
}

void ServerCore::runAuctionWorker() {
    unsigned int tid;
    auctionWorker = (HANDLE)_beginthreadex(NULL, 0, runAuctionWorkerThread, (void*)this, 0, &tid);
}
