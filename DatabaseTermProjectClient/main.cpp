#include <iostream>
#include <string>
#include <winsock2.h>
#include <process.h>

using namespace std;

string serverIP = "127.0.0.1";
int serverPort = 7707;
string nickname = "12345";

SOCKET clientInit() {
    SOCKET clientSocket;
    WSADATA wsadata;
    SOCKADDR_IN serverAddress;

    // WSAStartup
    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsadata) != 0) {
        cout << "WSAStartup Error." << endl;
        return 0;
    }

    // create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cout << "Socket Creation Error" << endl;
        return 0;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddress.sin_port = htons(serverPort);

    if ((connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress))) < 0) {
        cout << "connection error" << endl;
        return 0;
    }

    return clientSocket;
}

unsigned int WINAPI run(void* params) {
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

int main() {
    unsigned int tid;

    SOCKET clientSocket = clientInit();
    if (clientSocket == 0) {
        cout << "client Init fail" << endl;
        return 0;
    }

    HANDLE mainThread = (HANDLE)_beginthreadex(NULL, 0, run, (void*)clientSocket, 0, &tid);

    while (true) {
        cout << "message:";
        string msg; cin >> msg;
        send(clientSocket, msg.c_str(), sizeof(msg.c_str()), 0);
        if (msg == "exit") break;
    }

    closesocket(clientSocket);
    WSACleanup();
    CloseHandle(mainThread);

    return 0;
}
