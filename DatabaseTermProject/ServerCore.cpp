#include "ServerCore.h"

ServerCore::ServerCore() :  maxClientCount(serverConstant.getMaxClientCount()),
                            serverPort(serverConstant.getServerPort()),
                            serverIP(serverConstant.getServerIP()) {
    socketArray = shared_ptr<SOCK_INFO[]>(new SOCK_INFO[maxClientCount + 1]);
    socketCount = 0;
}

SOCKET ServerCore::initServer() {
    WSADATA wsadata;
    SOCKET sc;
    SOCKADDR_IN socketAddress;

    // WSAStartup
    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsadata) != 0) {
        cout << "WSAStartup Error." << endl;
        return 0;
    }

    // create socket
    if ((sc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cout << "socket creation error." << endl;
        return 0;
    }

    // bind
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    socketAddress.sin_port = htons(serverPort);
    if (bind(sc, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(socketAddress)) < 0) {
        cout << "bind error." << "\n";
        return 0;
    }

    // listen
    if (listen(sc, SOMAXCONN) < 0) {
        cout << "listen error." << endl;
        return 0;
    }

    return sc;
}

void ServerCore::run() {
    WSANETWORKEVENTS ev;
    WSAEVENT handleArray[maxClientCount + 1];

    SOCKET sc = initServer();
    if (sc == 0) {
        cout << "socket initialization error" << endl;
        exit(0);
    }

    HANDLE event = WSACreateEvent();
    socketArray[socketCount].ev         = event;
    socketArray[socketCount].sc         = sc;
    socketArray[socketCount].nickname   = "svr";
    socketArray[socketCount].clientIP   = "0.0.0.0";

    WSAEventSelect(sc, event, FD_ACCEPT);
    socketCount++;

    int index;
    while (true) {
        for (int i=0; i<socketCount; i++)
            handleArray[i] = socketArray[i].ev;

        index = WSAWaitForMultipleEvents(socketCount, handleArray, false, INFINITE, false);
        if ((index != WSA_WAIT_FAILED) and (index != WSA_WAIT_TIMEOUT)) {
            WSAEnumNetworkEvents(socketArray[index].sc, socketArray[index].ev, &ev);
            if (ev.lNetworkEvents == FD_ACCEPT) addClient();
            else if (ev.lNetworkEvents == FD_READ) readClient(index);
            else if (ev.lNetworkEvents == FD_CLOSE) removeClient(index);
        }
    }
    closesocket(sc);
    WSACleanup();
    _endthreadex(0);
}

void ServerCore::addClient() {
    SOCKADDR_IN socketAddress;
    SOCKET acceptSocket;
    int length;

    length = sizeof(socketAddress);
    acceptSocket = accept(socketArray[0].sc, reinterpret_cast<SOCKADDR*>(&socketAddress), &length);

    HANDLE event = WSACreateEvent();
    socketArray[socketCount].ev = event;
    socketArray[socketCount].sc = acceptSocket;
    socketArray[socketCount].clientIP = inet_ntoa(socketAddress.sin_addr);

    WSAEventSelect(acceptSocket, event, FD_READ | FD_CLOSE);

    notifyClient("New Client Connected (IP : " + socketArray[socketCount].clientIP + ", name : " + socketArray[socketCount].nickname + ")");
    socketCount++;
}

void ServerCore::readClient(int index) {
    char buf[MAXBYTE];
    SOCKADDR_IN clientAddress;
    int bytes = recv(socketArray[index].sc, buf, MAXBYTE, 0);
    notifyClient("(" + socketArray[index].clientIP + ")" + socketArray[index].nickname + " : " + buf);
}

void ServerCore::removeClient(int index) {
    string removeClientIP = socketArray[index].clientIP;
    string removeClientNickName = socketArray[index].nickname;
    socketCount--;
    swap(socketArray[index], socketArray[socketCount]);
    notifyClient("Client Disconnected (IP : " + removeClientIP + ", name : " + removeClientNickName + ")");
}

void ServerCore::notifyClient(string msg) {
    cout << msg << endl;
    for (int i=1; i<socketCount; i++)
        send(socketArray[i].sc, msg.c_str(), MAXBYTE, 0);
}
