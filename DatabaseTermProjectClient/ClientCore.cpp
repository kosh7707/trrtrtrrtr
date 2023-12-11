#include "ClientCore.h"

ClientCore::ClientCore() : serverIP(clientConstant.getServerIP()), serverPort(clientConstant.getServerPort()), isLogin(false) {
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
    connectDB();
    mainThread = (HANDLE)_beginthreadex(NULL, 0, runThread, (void*)sc, 0, &tid);
}

ClientCore::~ClientCore() {
    closesocket(sc);
    CloseHandle(mainThread);
    WSACleanup();
}

void ClientCore::login() {
    char buf[MAXBYTE];
    int length = 0;
    int index = 0;
    WSANETWORKEVENTS ev;
    HANDLE event = WSACreateEvent();

    WSAEventSelect(sc, event, FD_READ | FD_CLOSE);
    while (true) {
        string id, pw, role;
        cout << "please enter the id:"; cin >> id;
        cout << "please enter the pw:"; cin >> pw;
        cout << "please enter the role(0: merchant, 1: mage, 2: hacker):"; cin >> role;
        string loginRequest = "[0]" + id + "," + pw + "," + role;
        send(sc, loginRequest.c_str(), sizeof(loginRequest), 0);

        index = WSAWaitForMultipleEvents(1, &event, false, INFINITE, false);
        if ((index != WSA_WAIT_FAILED) and (index != WSA_WAIT_TIMEOUT)) {
            WSAEnumNetworkEvents(sc, event, &ev);
            if (ev.lNetworkEvents == FD_READ) {
                length = recv(sc, buf, MAXBYTE, 0);
                if (length > 0) {
                    string loginResponse = buf;
                    if (loginResponse == "200") {
                        cout << "login success" << endl;
                        userId = id;
                        userPw = pw;
                        isLogin = true;
                        break;
                    }
                    else if (loginResponse == "201") {
                        cout << "register success" << endl;
                        userId = id;
                        userPw = pw;
                        isLogin = true;
                        break;
                    }
                    else if (loginResponse == "400") {
                        cout << "wrong password, login failed" << endl;
                    }
                    else if (loginResponse == "401") {
                        cout << "register failed" << endl;
                    }
                }
            }
            else if (ev.lNetworkEvents == FD_CLOSE) {
                cout << "Server Disconnected" << endl;
                closesocket(sc);
                break;
            }
        }
    }
}

bool ClientCore::getIsLogin() {
    return isLogin;
}

void ClientCore::handleChat(const string& msg) {
    string temp = "[1]" + msg;
    send(sc, temp.c_str(), sizeof(temp), 0);
}

void ClientCore::handleCommand(const vector<string>& command) {
    if (command[0] == "getTestItem") {
        string cmd = "[2]";
        send(sc, cmd.c_str(), sizeof(cmd), 0);
    }
    else if (command[0] == "inventoryCheck") {
        string cmd = "[3]";
        send(sc, cmd.c_str(), sizeof(cmd), 0);
    }
    else if (command[0] == "sellItem") {
        string cmd = "[4]" + command[1] + "," + command[2] + "," + command[3] + "," + command[4];
        send(sc, cmd.c_str(), sizeof(cmd), 0);
    }
    else if (command[0] == "buyNow") {
        string cmd = "[5]" + command[1];
        send(sc, cmd.c_str(), sizeof(cmd), 0);
    }
    else if (command[0] == "bid") {
        string cmd = "[6]" + command[1] + "," + command[2];
        send(sc, cmd.c_str(), sizeof(cmd), 0);
    }
    else if (command[0] == "breakItem") {
        string cmd = "[7]" + command[1] + "," + command[2];
        send(sc, cmd.c_str(), sizeof(cmd), 0);
    }
    else if (command[0] == "query") {
        IDatabaseConnection& dc = DatabaseConnection::getInstance();
        cout << "Enter the SQL query (you need to input ; at the end of query.)\n";
        string query, temp;
        while (temp[temp.length()-1] != ';') {
            getline(cin, temp);
            query += temp + " ";
        }
        for (int i=0; i<=6; i++) query[i] = tolower(query[i]);
        if (query.substr(0, 6) == "select") dc.selectQuery(query);
        else dc.commandQuery(query);
    }
    else if (command[0] == "openPermissionStore") {
        string cmd = "[8]";
        send(sc, cmd.c_str(), sizeof(cmd), 0);
    }
    else if (command[0] == "buyPermission") {
        string cmd = "[9]" + command[1];
        send(sc, cmd.c_str(), sizeof(cmd), 0);
    }
    else {
        cout << "Invalid Command" << endl;
    }
}

const string &ClientCore::getUserId() const {
    return userId;
}

const string &ClientCore::getUserPw() const {
    return userPw;
}

void ClientCore::connectDB() {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    databaseConnection.connectDB(userId, userPw);
}

