#include "../include/ServerCore.h"

ServerCore::ServerCore(bool isServer, std::unique_ptr<IEventHandler> eventHandler, std::string serverIP, int serverPort)
                : serverIP(serverIP), serverPort(serverPort), eventHandler(std::move(eventHandler)), isServer(isServer) {
    connectedSockets = std::unique_ptr<Socket[]>(new Socket[100]);
    clientsCount = 0;
}

[[noreturn]] void ServerCore::run() {
    initServer();

    if (sc.getSc() == 0) {
        std::cout << "socket initialization error" << std::endl;
        exit(-1);
    }

    if (isServer) WSAEventSelect(sc.getSc(), sc.getEv(), FD_ACCEPT);
    else {
        if (!connect(serverIP, serverPort))
            exit(-1);
    }

    runRecvWorker();
    runEventHandlingWorker();
    runSendWorker();

    if (isServer) {
        std::cout << "server listening at "
                  << serverIP << ":"
                  << serverPort
                  << std::endl;
    }
    else {
        std::cout << "successfully connected to "
                  << serverIP << ":"
                  << serverPort
                  << std::endl;
    }

    while (true) {
        try {
            std::string command; getline(std::cin, command);
            if (command == "") continue;
            else {
                auto res = eventHandler->userInputHandling(command);
                for (auto& it : res)
                    eventResQueue.push(std::move(it));
            }
        } catch(const std::exception& e) {
            std::cerr << e.what() << "\n";
        }
    }
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

    if (isServer) {
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
    }

    HANDLE ev = WSACreateEvent();
    sc = Socket(_sc, ev, "0.0.0.0");
    return true;
}

bool ServerCore::accept() {
    if (!isServer) return false;
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
    std::cout << "socket: " << socket.getSc() << ", IP: " << socket.getIp() << " is connected." << std::endl;
    publish();
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
    publish();
    return true;
}

bool ServerCore::close(const int index) {
    std::string close_ip = connectedSockets[index].getIp();
    SOCKET      close_sc = connectedSockets[index].getSc();
    clientsCount = clientsCount - 1;
    std::swap(connectedSockets[index], connectedSockets[clientsCount]);
    ::close(connectedSockets[clientsCount].getSc());
    std::cout << "socket: " << close_sc << ", IP: " << close_ip << " is disconnected." << std::endl;
    publish();
    return true;
}

bool ServerCore::recv(const int index) {
    char header[5];
    memset(header, 0, sizeof(header));

    int bytesReceived = 0;
    while (bytesReceived < sizeof(header)) {
        int tempReceived = ::recv(connectedSockets[index].getSc(), header + bytesReceived, sizeof(header) - bytesReceived, 0);
        if (tempReceived <= 0) {
            std::cerr << "recv header error, errno: " << WSAGetLastError() << std::endl;
            return false;
        }
        bytesReceived += tempReceived;
    }

    uint8_t eventCode = static_cast<uint8_t>(header[0]);

    uint32_t length;
    memcpy(&length, &header[1], sizeof(length));
    length = ntohl(length);

    if (length > BUF_SIZE) {
        std::cerr << "recv error, message length exceeds BUF_SIZE" << std::endl;
        return false;
    }

    char buf[BUF_SIZE];
    bytesReceived = 0;
    while (bytesReceived < static_cast<int>(length)) {
        int tempReceived = ::recv(connectedSockets[index].getSc(), buf + bytesReceived, static_cast<int>(length) - bytesReceived, 0);
        if (tempReceived <= 0) {
            std::cerr << "recv contents error, errno: " << WSAGetLastError() << std::endl;
            return false;
        }
        bytesReceived += tempReceived;
    }

    std::string msg(buf, bytesReceived);
    auto pEvent = std::make_unique<Event>(index, eventCode, msg);
    eventReqQueue.push(std::move(pEvent));

    return true;
}

bool ServerCore::send(std::unique_ptr<Event> event) {
    int         index       = event->getIndex();
    uint8_t     eventCode   = event->getEventCode();
    std::string contents    = event->getContents();

    uint32_t length = htonl(static_cast<uint32_t>(contents.length()));

    std::string msg;
    msg.push_back(static_cast<char>(eventCode));
    msg.append(reinterpret_cast<const char*>(&length), sizeof(length)).append(contents);

    const char* buf = msg.c_str();
    int totalLength = static_cast<int>(msg.length());
    int bytesSent = 0;

    while (bytesSent < totalLength) {
        int tempSent = ::send(connectedSockets[index].getSc(), buf + bytesSent, totalLength - bytesSent, 0);
        if (tempSent <= 0) {
            std::cerr << "send error, errno: " << WSAGetLastError() << std::endl;
            return false;
        }
        bytesSent += tempSent;
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
            if (index == 0) {
                WSAEnumNetworkEvents(serverCore->sc.getSc(), serverCore->sc.getEv(), &ev);
                if (ev.lNetworkEvents == FD_ACCEPT) serverCore->accept();
            }
            else {
                index = index - 1;
                Socket& sc = serverCore->connectedSockets[index];
                WSAEnumNetworkEvents(sc.getSc(), sc.getEv(), &ev);
                if (ev.lNetworkEvents == FD_READ) serverCore->recv(index);
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
        std::unique_ptr<Event> event;
        serverCore->eventReqQueue.pop(event);
        auto res = serverCore->eventHandler->handling(std::move(event));
        for (auto& it : res)
            serverCore->eventResQueue.push(std::move(it));
    }
}

void ServerCore::runSendWorker() {
    unsigned int tid;
    sendWorker = (HANDLE)_beginthreadex(NULL, 0, runSendWorkerThread, (void*)this, 0, &tid);
}

[[noreturn]] unsigned int WINAPI ServerCore::runSendWorkerThread(void* params) {
    ServerCore* serverCore = static_cast<ServerCore*>(params);

    while (true) {
        std::unique_ptr<Event> event;
        serverCore->eventResQueue.pop(event);
        serverCore->send(std::move(event));
    }
}

void ServerCore::attachSocketObserver(std::shared_ptr<IObserver> observer) {
    observers.emplace_back(observer);
}

void ServerCore::publish() {
    for (const auto& observer : observers)
        observer->update(clientsCount, connectedSockets);
}





