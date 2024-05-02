#include "ServerCore.h"

ServerCore::ServerCore(std::string serverIP, int serverPort, std::unique_ptr<IEventHandler> eventHandler)
                : serverIP(serverIP), serverPort(serverPort), eventHandler(std::move(eventHandler)) {
    connectedSockets = std::unique_ptr<Socket[]>(new Socket[100]);
    clientsCount = 0;
}

[[noreturn]] void ServerCore::run() {
    initServer();

    if (sc.getSc() == 0) {
        std::cout << "socket initialization error" << std::endl;
        exit(0);
    }

    WSAEventSelect(sc.getSc(), sc.getEv(), FD_ACCEPT);

    runRecvWorker();
    runEventHandlingWorker();
    runSendWorker();

    std::cout << "server listening at "
              << serverIP << ":"
              << serverPort
              << std::endl;

    while (true);
}

bool ServerCore::initServer() {
    SOCKET _sc;
    WSADATA wsadata;
    SOCKADDR_IN socketAddress;

    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsadata) != 0) {
        std::cerr << "WSAStartup Error." << std::endl;
        return false;
    }

    if ((_sc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cerr << "socket creation error." << std::endl;
        return false;
    }

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    socketAddress.sin_port = htons(serverPort);
    if (bind(_sc, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(socketAddress)) < 0) {
        std::cerr << "bind error." << "\n";
        return false;
    }

    if (listen(_sc, SOMAXCONN) < 0) {
        std::cerr << "listen error." << std::endl;
        return false;
    }

    HANDLE ev = WSACreateEvent();
    sc = Socket(_sc, ev, "0.0.0.0");
    return true;
}

bool ServerCore::accept() {
    SOCKADDR_IN socketAddress;
    int length = sizeof(socketAddress);
    SOCKET acceptSocket = ::accept(sc.getSc(), reinterpret_cast<SOCKADDR*>(&socketAddress), &length);
    if (acceptSocket == 0) {
        std::cerr << "accept Error" << std::endl;
        return false;
    }
    HANDLE ev = WSACreateEvent();
    Socket socket(acceptSocket, ev, inet_ntoa(socketAddress.sin_addr));
    WSAEventSelect(socket.getSc(), socket.getEv(), FD_READ | FD_CLOSE);
    connectedSockets[clientsCount++] = socket;
    return true;
}

bool ServerCore::connect(const std::string& destServerIP, const int destServerPort) {
    SOCKET connectSocket;
    WSADATA wsadata;
    SOCKADDR_IN serverAddress;
    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsadata) != 0) {
        std::cerr << "WSAStartup Error." << std::endl;
        return false;
    }

    if ((connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cerr << "Socket Creation Error" << std::endl;
        return false;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(destServerIP.c_str());
    serverAddress.sin_port = htons(destServerPort);
    if ((::connect(connectSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress))) < 0) {
        std::cerr << "Connection Error" << std::endl;
        return false;
    }

    HANDLE ev = WSACreateEvent();
    Socket socket(connectSocket, ev, destServerIP);
    WSAEventSelect(socket.getSc(), socket.getEv(), FD_READ | FD_CLOSE);
    connectedSockets[clientsCount++] = socket;
    return true;
}

bool ServerCore::close(const int index) {
    ::close(connectedSockets[--clientsCount].getSc());
    std::swap(connectedSockets[index], connectedSockets[clientsCount]);
    return true;
}

bool ServerCore::read(const int index) {
    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));
    if (recv(connectedSockets[index].getSc(), buf, BUF_SIZE, 0) <= 0) {
        std::cerr << "recv error, errno: " << WSAGetLastError() << "\n";
        return false;
    }
    // TODO: 패킷 헤더를 만들고, 그에 따라 읽는 byte를 정하도록 바꿔야 함.
    // 현재는 2byte로 코드를 나눔. "00message"
    int eventCode = (buf[0] - '0') * 10 + (buf[1] - '0');
    eventReqQueue.push({index, eventCode, static_cast<std::string>(buf)});
    return true;
}

bool ServerCore::send(const Event& event) {
    const int index = event.getIndex();
    const std::string& msg = event.getContents();
    if (index == 0) {
        for (int i=0; i<clientsCount; i++) {
            if (::send(connectedSockets[i].getSc(), msg.c_str(), (int)msg.length(), 0) <= 0) {
                std::cerr << "send error, errno: " << WSAGetLastError() << "\n";
                return false;
            }
        }
    }
    else {
        if (::send(connectedSockets[index].getSc(), msg.c_str(), (int)msg.length(), 0) <= 0) {
            std::cerr << "send error, errno: " << WSAGetLastError() << "\n";
            return false;
        }
    }
    return true;
}


void ServerCore::runRecvWorker() {
    unsigned int tid;
    recvWorker = (HANDLE)_beginthreadex(NULL, 0, runRecvWorkerThread, (void*)this, 0, &tid);
}

[[noreturn]] unsigned int WINAPI ServerCore::runRecvWorkerThread(void* params) {
    ServerCore* serverCore = static_cast<ServerCore*>(params);

    WSANETWORKEVENTS ev;
    WSAEVENT handleArray[100];

    int index;
    while (true) {
        handleArray[0] = serverCore->sc.getEv();
        for (int i=0; i<serverCore->clientsCount; i++)
            handleArray[i+1] = serverCore->connectedSockets[i].getEv();
        index = WSAWaitForMultipleEvents(serverCore->clientsCount+1, handleArray, false, INFINITE, false);
        if ((index != WSA_WAIT_FAILED) and (index != WSA_WAIT_TIMEOUT)) {
            if (index == 0) serverCore->accept();
            else {
                index = index - 1;
                Socket& sc = serverCore->connectedSockets[index];
                WSAEnumNetworkEvents(sc.getSc(), sc.getEv(), &ev);
                if (ev.lNetworkEvents == FD_READ) serverCore->read(index);
                else if (ev.lNetworkEvents == FD_CLOSE) serverCore->close(index);
            }
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

        Event event = serverCore->eventReqQueue.front(); serverCore->eventReqQueue.pop();
        auto res = serverCore->eventHandler->handling(event);
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
        serverCore->send(event);
    }
}


