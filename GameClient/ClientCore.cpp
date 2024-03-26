#include "ClientCore.h"

ClientCore::ClientCore() : serverIP(clientConstant.getServerIP()), serverPort(clientConstant.getServerPort()), isLogin(false) {}

void ClientCore::initClient() {
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

void ClientCore::run() {
    unsigned int tid;
    mainThread = (HANDLE)_beginthreadex(NULL, 0, runThread, (void*)this, 0, &tid);
}

unsigned int WINAPI ClientCore::runThread(void* params) {
    ClientCore* clientCore = static_cast<ClientCore*>(params);

    int index;
    WSANETWORKEVENTS ev;
    HANDLE event = WSACreateEvent();
    WSAEventSelect(clientCore->sc, event, FD_READ | FD_CLOSE);
    while (true) {
        index = WSAWaitForMultipleEvents(1, &event, false, INFINITE, false);
        if ((index != WSA_WAIT_FAILED) and (index != WSA_WAIT_TIMEOUT)) {
            WSAEnumNetworkEvents(clientCore->sc, event, &ev);
            if (ev.lNetworkEvents == FD_READ) clientCore->ReadServer();
            else if (ev.lNetworkEvents == FD_CLOSE) break;
        }
    }
    _endthreadex(0);
}

int ClientCore::getEvent(const char* buf) {
    string temp = buf;
    string prefix = temp.substr(0, 3);
    if (prefix == "[0]") return LOGIN_EVENT;
    else if (prefix == "[1]") return CHAT_EVENT;
    else return INVALID_EVENT;
}

string ClientCore::getMessage(const char* buf) {
    string temp = buf;
    return temp.substr(3);
}

void ClientCore::handleChat(const string& msg) {
    cout << msg << "\n";
}

void ClientCore::handleLogin(const string& msg) {
    string loginResponse = msg;
    if (loginResponse == "000") {
        cout << "login success" << endl;
        isLogin = true;
    }
    else if (loginResponse == "001") {
        cout << "register success" << endl;
        isLogin = true;
    }
    else if (loginResponse == "002") {
        cout << "wrong password, login failed" << endl;
    }
    else if (loginResponse == "003") {
        cout << "register failed" << endl;
    }
}

void ClientCore::ReadServer() {
    char buf[BUF_SIZE];
    if (recv(sc, buf, BUF_SIZE, 0) <= 0)
        std::cerr << "recv error, errno: " << WSAGetLastError() << "\n";
    cout << "[Recv]: " << buf << "\n";
    int event = getEvent(buf); string msg = getMessage(buf);
    if (event == LOGIN_EVENT) handleLogin(msg);
    else if (event == CHAT_EVENT) handleChat(msg);
    else cout << "INVALID EVENT\nmsg: " << msg << endl;
}

void ClientCore::login(const string& id, const string& pw) {
    userId = id;
    userPw = pw;
    string loginRequest = "[0]" + id + "," + pw;
    notifyServer(loginRequest);

    int index;
    WSANETWORKEVENTS ev;
    HANDLE event = WSACreateEvent();
    WSAEventSelect(sc, event, FD_READ | FD_CLOSE);
    while (true) {
        index = WSAWaitForMultipleEvents(1, &event, false, INFINITE, false);
        if ((index != WSA_WAIT_FAILED) and (index != WSA_WAIT_TIMEOUT)) {
            WSAEnumNetworkEvents(sc, event, &ev);
            if (ev.lNetworkEvents == FD_READ) {
                ReadServer();
                break;
            }
            else if (ev.lNetworkEvents == FD_CLOSE) break;
        }
    }
}

bool ClientCore::getIsLogin() {
    return isLogin;
}

void ClientCore::sendChat(const std::string &msg) {
    notifyServer("[1]" + msg);
}

void ClientCore::sendCommand(const vector<std::string> &command) {
    try {
        if (command[0] == "getTestItem") {
            notifyServer("[2]");
        }
        else if (command[0] == "inventoryCheck") {
            notifyServer("[3]");
        }
        else if (command[0] == "sellItem") {
            notifyServer("[4]" + command[1] + "," + command[2] + "," + command[3] + "," + command[4]);
        }
        else if (command[0] == "buyNow") {
            notifyServer("[5]" + command[1]);
        }
        else if (command[0] == "bid") {
            notifyServer("[6]" + command[1] + "," + command[2]);
        }
        else if (command[0] == "breakItem") {
            notifyServer("[7]" + command[1] + "," + command[2]);
        }
        else if (command[0] == "auctionCheck") {
            notifyServer("[8]");
        }
        else {
            cout << "Invalid Command" << endl;
        }
    } catch (const exception& e) {
        cerr << "Error, " << e.what() << endl;
        cerr << "command 처리 실패" << endl;
    }
}

void ClientCore::notifyServer(const string& msg) {
    if (send(sc, msg.c_str(), BUF_SIZE, 0) <= 0)
        std::cerr << "send error, errno: " << WSAGetLastError() << "\n";
}

const string &ClientCore::getUserId() const {
    return userId;
}

const string &ClientCore::getUserPw() const {
    return userPw;
}

