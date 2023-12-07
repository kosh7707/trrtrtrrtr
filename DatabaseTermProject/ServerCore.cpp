#include "ServerCore.h"

ServerCore::ServerCore() :  maxClientCount(serverConstant.getMaxClientCount()),
                            serverPort(serverConstant.getServerPort()),
                            serverIP(serverConstant.getServerIP()) {
    Clients = shared_ptr<Client[]>(new Client[maxClientCount + 1]);
    ClientsCount = 0;
}

SOCKET ServerCore::initServer() {
    WSADATA wsadata;
    SOCKET sc;
    SOCKADDR_IN socketAddress;

    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsadata) != 0) {
        cout << "WSAStartup Error." << endl;
        return 0;
    }

    if ((sc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cout << "socket creation error." << endl;
        return 0;
    }

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    socketAddress.sin_port = htons(serverPort);
    if (bind(sc, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(socketAddress)) < 0) {
        cout << "bind error." << "\n";
        return 0;
    }

    if (listen(sc, SOMAXCONN) < 0) {
        cout << "listen error." << endl;
        return 0;
    }

    return sc;
}

[[noreturn]] void ServerCore::run() {
    WSANETWORKEVENTS ev;
    WSAEVENT handleArray[maxClientCount + 1];

    SOCKET sc = initServer();
    if (sc == 0) {
        cout << "socket initialization error" << endl;
        exit(0);
    }

    HANDLE event = WSACreateEvent();
    Clients[ClientsCount].setEv(event);
    Clients[ClientsCount].setSc(sc);
    Clients[ClientsCount].setIp("0.0.0.0");

    WSAEventSelect(sc, event, FD_ACCEPT);
    ClientsCount++;

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

void ServerCore::removeClient(const int index) {
    string removeClientIP = Clients[index].getIp();
    string removeClientNickName = Clients[index].getAccount()->getUserId();
    ClientsCount--;
    swap(Clients[index], Clients[ClientsCount]);
    notifyAllClients("Client Disconnected (IP: " + removeClientIP + ", name: " + removeClientNickName + ")");
}

void ServerCore::notifyAllClients(const string& msg) {
    cout << msg << endl;
    for (int i=1; i<ClientsCount; i++)
        send(Clients[i].getSc(), msg.c_str(), MAXBYTE, 0);
}

void ServerCore::notifyClient(const int index, const string& msg) {
    send(Clients[index].getSc(), msg.c_str(), MAXBYTE, 0);
}

int ServerCore::getEvent(const char* buf) {
    string temp = buf;
    string prefix = temp.substr(0, 3);
    if (prefix == "[0]") return LOGIN_EVENT;
    else if (prefix == "[1]") return CHAT_EVENT;
    else if (prefix == "[2]") return GET_TEST_ITEM_EVENT;
    else if (prefix == "[3]") return INVENTORY_CHECK_EVENT;
    else return INVALID_EVENT;
}

string ServerCore::getMessage(const char* buf) {
    string temp = buf;
    return temp.substr(3);
}

/*-------------------
| Event Handling Part
-------------------*/

void ServerCore::readClient(const int index) {
    char buf[MAXBYTE];
    recv(Clients[index].getSc(), buf, MAXBYTE, 0);
    int event = getEvent(buf); string msg = getMessage(buf);
    if (event == LOGIN_EVENT) handleLogin(index, msg);
    else if (event == CHAT_EVENT) handleChat(index, msg);
    else if (event == GET_TEST_ITEM_EVENT) handleGetTestItem(index, msg);
    else if (event == INVENTORY_CHECK_EVENT) handleInventoryCheck(index, msg);
    else cout << "INVALID EVENT\nmsg: " << msg << endl;
}

void ServerCore::handleChat(const int index, const string& msg) {
    notifyAllClients(Clients[index].getAccount()->getUserId() + " : " + msg);
}

void ServerCore::handleLogin(const int index, const string& msg) {
    string temp;
    vector<string> vec;
    for (int i=0; i<msg.length(); i++) {
        if (msg[i] == ',') {
            vec.emplace_back(temp);
            temp = "";
        }
        else temp += msg[i];
    }
    vec.emplace_back(temp);
    string id = vec[0];
    string pw = vec[1];
    string role = vec[2];
    if (accountDao.checkAccountExists(id)) {
        // login
        shared_ptr<Account> account = accountDao.getAccount(id, pw, role);
        if (account != nullptr) {
            Clients[index].setAccount(account);
            accountDao.updateAccountLastLogin(id);
            notifyClient(index, "200");
            notifyAllClients("New Client Connected (IP: " + Clients[index].getIp() + ", name: " + Clients[index].getAccount()->getUserId() + ")");
        }
        else {
            notifyClient(index, "400");
        }
    }
    else {
        // register
        bool res = accountDao.registerAccount(id, pw, role);
        if (res) {
            shared_ptr<Account> account = accountDao.getAccount(id, pw, role);
            Clients[index].setAccount(account);
            notifyClient(index, "201");
            notifyAllClients("New Client Connected (IP: " + Clients[index].getIp() + ", name: " + Clients[index].getAccount()->getUserId() + ")");
        }
        else {
            notifyClient(index, "401");
        }
    }
}

void ServerCore::handleGetTestItem(const int index) {
    string id = Clients[index].getAccount()->getUserId();
    bool res = inventoryDao.insertItem(id, 1);
    if (res) cout << "grant user '" + id + "' test item." << endl;
    else cout << "error, can't grant user '" + id + "' test item." << endl;
}

void ServerCore::handleInventoryCheck(const int index) {
    string id = Clients[index].getAccount()->getUserId();
    bool res = inventoryDao.inventoryCheck(id);
    // TODO 인벤토리 내용물 msg로 담아서 send 구현
}

