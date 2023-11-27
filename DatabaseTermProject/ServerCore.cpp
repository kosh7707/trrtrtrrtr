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
        return -1;
    }

    // create socket
    if ((sc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cout << "socket creation error." << endl;
        return -1;
    }

    // bind
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    socketAddress.sin_port = htons(serverPort);
    if (bind(sc, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(socketAddress)) < 0) {
        cout << "bind error." << "\n";
        return -1;
    }

    // listen
    if (listen(sc, SOMAXCONN) < 0) {
        cout << "listen error." << endl;
        return -1;
    }

    return sc;
}

int ServerCore::run() {
    WSANETWORKEVENTS ev;
    int index;
    WSAEVENT handle_array[maxClientCount + 1];

    SOCKET sc = initServer();
    if (sc < 0) {
        cout << "socket initialization error" << endl;
        return -1;
    }
}

