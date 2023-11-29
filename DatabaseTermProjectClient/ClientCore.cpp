#include "ClientCore.h"

ClientCore::ClientCore() : serverIP(clientConstant.getServerIP()), serverPort(clientConstant.getServerPort()) {
    WSADATA wsadata;
    SOCKADDR_IN serverAddress;

    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsadata) != 0) {
        cout << "WSAStartup Error." << endl;
        exit(0);
    }

    if ((sc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cout << "Socket Creation Error" << endl;
        exit(0);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddress.sin_port = htons(serverPort);

    if ((connect(sc, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress))) < 0) {
        cout << "Connection Error" << endl;
        exit(0);
    }
}

SOCKET ClientCore::getSocket() {
    return sc;
}

unsigned int WINAPI ClientCore::runThread(void* params) {
    SOCKET sc = (SOCKET)params;
    char buf[MAXBYTE];
    int length = 0;
    int index = 0;
    WSANETWORKEVENTS ev;
    HANDLE event = WSACreateEvent();

    WSAEventSelect(sc, event, FD_READ | FD_CLOSE);
    while (true) {
        index = WSAWaitForMultipleEvents(1, &event, false, INFINITE, false);
        if ((index != WSA_WAIT_FAILED) and (index != WSA_WAIT_TIMEOUT)) {
            WSAEnumNetworkEvents(sc, event, &ev);
            if (ev.lNetworkEvents == FD_READ) {
                length = recv(sc, buf, MAXBYTE, 0);
                if (length > 0) cout << buf << endl;
            }
            else if (ev.lNetworkEvents == FD_CLOSE) {
                cout << "Server Disconnected" << endl;
                closesocket(sc);
                break;
            }
        }
    }
    WSACleanup();
    _endthreadex(0);
}

void ClientCore::run() {
    mainThread = (HANDLE)_beginthreadex(NULL, 0, runThread, (void*)sc, 0, &tid);
}

ClientCore::~ClientCore() {
    closesocket(sc);
    CloseHandle(mainThread);
    WSACleanup();
}

void ClientCore::login() {
    char buf[MAXBYTE];
    string id, pw;
    cout << "please enter the id:"; cin >> id;
    cout << "please enter the pw:"; cin >> pw;
    string loginRequest = "[login]" + id + "," + pw;
    send(sc, loginRequest.c_str(), sizeof(loginRequest), 0);
    recv(sc, buf, MAXBYTE, 0);
    string loginResponse = buf;
    if (loginResponse == "200") {
        // login success
    }
    else if (loginResponse == "201") {
        // register success
    }
    else {
        // login error
    }
}
