#include "ClientCore.h"

ClientCore::ClientCore() : serverIP(clientConstant.getServerIP()), serverPort(clientConstant.getServerPort())
                            , isLogin(false) {}

[[noreturn]] void ClientCore::run() {
    initClient();
    if (sc == 0) {
        std::cout << "socket initialization error" << std::endl;
        exit(0);
    }

    runRecvWorker();
    runEventHandlingWorker();
    runSendWorker();

    std::cout << "----- online -----\n";
    while (true) {
        std::cin.ignore();
        std::string command; getline(std::cin, command);
        if (command == "!help") printUserCommand();
        else eventResQueue.push(eventHandler.userEventHandling(isLogin, command));
    }
}

void ClientCore::initClient() {
    WSADATA wsadata;
    SOCKADDR_IN serverAddress;

    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsadata) != 0) {
        std::cout << "WSAStartup Error." << std::endl;
        exit(0);
    }

    if ((sc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cout << "Socket Creation Error" << std::endl;
        exit(0);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddress.sin_port = htons(serverPort);
    if ((connect(sc, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress))) < 0) {
        std::cout << "Connection Error" << std::endl;
        exit(0);
    }
}

void ClientCore::notifyServer(const std::string& msg) {
    if (send(sc, msg.c_str(), BUF_SIZE, 0) <= 0)
        std::cerr << "send error, errno: " << WSAGetLastError() << "\n";
}

void ClientCore::runRecvWorker() {
    unsigned int tid;
    recvWorker = (HANDLE)_beginthreadex(NULL, 0, runRecvWorkerThread, (void*)this, 0, &tid);
}

void ClientCore::printUserCommand() {
    if (!isLogin) {
        std::cout   << "------------------------\n"
                    << "0. !login [id] [pw]"
                    << "------------------------\n";
    }
    else {
        std::cout   << "------------------------\n"
                    << "1. !getTestItem\n"
                    << "2. !inventoryCheck\n"
                    << "3. !sellItem [item_id] [quantity] [buyNowPrice] [startingBidPrice]\n"
                    << "4. !buyNow [auction_id]\n"
                    << "5. !bid [auction_id] [price]\n"
                    << "6. !breakItem [item_id] [quantity]\n"
                    << "7. !auctionCheck\n"
                    << "------------------------\n";
    }
}

[[noreturn]] unsigned int WINAPI ClientCore::runRecvWorkerThread(void* params) {
    ClientCore* clientCore = static_cast<ClientCore*>(params);

    WSANETWORKEVENTS ev;
    HANDLE event = WSACreateEvent();
    WSAEventSelect(clientCore->sc, event, FD_READ | FD_CLOSE);

    int index;
    while (true) {
        index = WSAWaitForMultipleEvents(1, &event, false, INFINITE, false);
        if ((index != WSA_WAIT_FAILED) and (index != WSA_WAIT_TIMEOUT)) {
            WSAEnumNetworkEvents(clientCore->sc, event, &ev);
            if (ev.lNetworkEvents == FD_READ) {
                char buf[BUF_SIZE];
                if (recv(clientCore->sc, buf, BUF_SIZE, 0) <= 0)
                    std::cerr << "recv error, errno: " << WSAGetLastError() << "\n";
                clientCore->eventReqQueue.push(static_cast<std::string>(buf));
            }
            else if (ev.lNetworkEvents == FD_CLOSE) break;
        }
    }
}

void ClientCore::runEventHandlingWorker() {
    unsigned int tid;
    eventHandlingWorker = (HANDLE)_beginthreadex(NULL, 0, runEventHandlingWorkerThread, (void*)this, 0, &tid);
}

[[noreturn]] unsigned int WINAPI ClientCore::runEventHandlingWorkerThread(void* params) {
    ClientCore* clientCore = static_cast<ClientCore*>(params);

    while (true) {
        if (clientCore->eventReqQueue.empty()) continue;
        auto event = clientCore->eventReqQueue.front(); clientCore->eventReqQueue.pop();
        auto res = clientCore->eventHandler.serverEventHandling(clientCore->isLogin, event);
        for (auto it : res)
            clientCore->eventResQueue.push(it);
    }
}

void ClientCore::runSendWorker() {
    unsigned int tid;
    sendWorker = (HANDLE)_beginthreadex(NULL, 0, runSendWorkerThread, (void*)this, 0, &tid);
}

[[noreturn]] unsigned int WINAPI ClientCore::runSendWorkerThread(void* params) {
    ClientCore* clientCore = static_cast<ClientCore*>(params);

    while (true) {
        if (clientCore->eventResQueue.empty()) continue;
        auto event = clientCore->eventResQueue.front(); clientCore->eventResQueue.pop();
        clientCore->notifyServer(event);
    }
}


