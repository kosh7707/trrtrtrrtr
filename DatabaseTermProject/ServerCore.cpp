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

/*-------------------
| Client Handling Part
-------------------*/

[[noreturn]] void ServerCore::run() {
    runAuctionWorker();

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

    cout << "server listening at 127.0.0.1:7707" << endl;
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
    if (Clients[index].getAccount() == nullptr) {
        ClientsCount--;
        swap(Clients[index], Clients[ClientsCount]);
    }
    else {
        string removeClientIP = Clients[index].getIp();
        string removeClientNickName = Clients[index].getAccount()->getUserId();
        ClientsCount--;
        swap(Clients[index], Clients[ClientsCount]);
        notifyAllClients("Client Disconnected (IP: " + removeClientIP + ", name: " + removeClientNickName + ")");
    }
}

void ServerCore::notifyAllClients(const string& msg) {
    cout << msg << endl;
    for (int i=1; i<ClientsCount; i++)
        send(Clients[i].getSc(), msg.c_str(), BUF_SIZE, 0);
}

void ServerCore::notifyClient(const int index, const string& msg) {
    if (msg.length() >= BUF_SIZE) cerr << "Warning: Message may be truncated as it exceeds BUF_SIZE." << endl;
    send(Clients[index].getSc(), msg.c_str(), BUF_SIZE, 0);
}

int ServerCore::getEvent(const char* buf) {
    string temp = buf;
    string prefix = temp.substr(0, 3);
    if (prefix == "[0]") return LOGIN_EVENT;
    else if (prefix == "[1]") return CHAT_EVENT;
    else if (prefix == "[2]") return GET_TEST_ITEM_EVENT;
    else if (prefix == "[3]") return INVENTORY_CHECK_EVENT;
    else if (prefix == "[4]") return SELL_ITEM_EVENT;
    else if (prefix == "[5]") return BUY_NOW_EVENT;
    else if (prefix == "[6]") return BID_EVENT;
    else if (prefix == "[7]") return BREAK_ITEM_EVENT;
    else if (prefix == "[8]") return OPEN_PERMISSION_STORE_EVENT;
    else if (prefix == "[9]") return BUY_PERMISSION_EVENT;
    else return INVALID_EVENT;
}

string ServerCore::getMessage(const char* buf) {
    string temp = buf;
    return temp.substr(3);
}

vector<string> ServerCore::split(const string& input, char delimiter) {
    vector<string> result;
    stringstream ss(input);
    string token;
    while (getline(ss, token, delimiter))
        result.push_back(token);
    return result;
}

/*-------------------
| Event Handling Part
-------------------*/

unsigned int WINAPI ServerCore::runAuctionWorkerThread(void* params) {
    ServerCore* serverCore = static_cast<ServerCore*>(params);
    IDatabaseConnection& dc = DatabaseConnection::getInstance();
    while (dc.isConnected()) {
        auto res = dc.selectQuery("select * from auctions where end_time <= CURRENT_TIMESTAMP");
        for (auto row : res) {
            vector<string> queries;
            int auction_id = stoi(row["auction_id"]);
            int item_id = stoi(row["item_id"]);
            int seller_id = stoi(row["seller_id"]);
            int item_quantity = stoi(row["item_quantity"]);
            if (row["current_bidder_id"] != "") {
                int current_bidder_id = stoi(row["current_bidder_id"]);
                int current_price = stoi(row["current_price"]);
                const char* queryTemplate1 = "delete from auctions where auction_id = %d;";
                queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate1, auction_id));
                const char* queryTemplate2 = "update accounts set balance = balance + %d where account_id = %d;";
                queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate2, current_price, seller_id));
                const char* queryTemplate3 = "insert into inventory (account_id, item_id, quantity) values (%d, %d, %d)\n"
                                             "on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
                queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate3, current_bidder_id, item_id, item_quantity));
                if (dc.transaction(queries)) {
                    for (int i=1; i<serverCore->ClientsCount; i++) {
                        if (serverCore->Clients[i].getAccount()->getAccountId() == current_bidder_id)
                            serverCore->notifyClient(i, "You've won the auction item_id: " + to_string(item_id) + ". please check your inventory");
                        else if (serverCore->Clients[i].getAccount()->getAccountId() == seller_id)
                            serverCore->notifyClient(i, "your item has been sold out. please check your inventory");
                    }
                }
            }
            else {
                const char* queryTemplate1 = "delete from auctions where auction_id = %d;";
                queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate1, auction_id));
                const char* queryTemplate2 = "insert into inventory (account_id, item_id, quantity) values (%d, %d, %d)\n"
                                             "on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
                queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate2, seller_id, item_id, item_quantity));
                if (dc.transaction(queries)) {
                    for (int i=1; i<serverCore->ClientsCount; i++) {
                        if (serverCore->Clients[i].getAccount()->getAccountId() == seller_id)
                            serverCore->notifyClient(i, "your auction has been expired. please check your inventory");
                    }
                }
            }
        }
        this_thread::sleep_for(chrono::seconds(5));
    }
    return 0;
}

void ServerCore::runAuctionWorker() {
    unsigned int tid;
    auctionWorker = (HANDLE)_beginthreadex(NULL, 0, runAuctionWorkerThread, (void*)this, 0, &tid);
}

void ServerCore::readClient(const int index) {
    char buf[BUF_SIZE];
    recv(Clients[index].getSc(), buf, BUF_SIZE, 0);
    int event = getEvent(buf); string msg = getMessage(buf);
    if (event == LOGIN_EVENT) handleLogin(index, msg);
    else if (event == CHAT_EVENT) handleChat(index, msg);
    else if (event == GET_TEST_ITEM_EVENT) handleGetTestItem(index);
    else if (event == INVENTORY_CHECK_EVENT) handleInventoryCheck(index);
    else if (event == SELL_ITEM_EVENT) handleSellItem(index, msg);
    else if (event == BUY_NOW_EVENT) handleBuyNow(index, msg);
    else if (event == BID_EVENT) handleBid(index, msg);
    else if (event == BREAK_ITEM_EVENT) handleBreakItem(index, msg);
    else if (event == OPEN_PERMISSION_STORE_EVENT) handleOpenPermissionStore(index);
    else if (event == BUY_PERMISSION_EVENT) handleBuyPermission(index, msg);
    else cout << "INVALID EVENT\nmsg: " << msg << endl;
}

void ServerCore::handleChat(const int index, const string& msg) {
    notifyAllClients(Clients[index].getAccount()->getUserId() + " : " + msg);
}

void ServerCore::handleLogin(const int index, const string& msg) {
    vector<string> params = split(msg, ',');
    string id = params[0];
    string pw = params[1];
    string role = params[2];
    if (accountDao.checkAccountExists(id)) {
        // login
        shared_ptr<Account> account = accountDao.getAccount(id, pw, role);
        if (account != nullptr) {
            Clients[index].setAccount(account);
            accountDao.updateAccountLastLogin(id);
            notifyClient(index, "200");
            notifyAllClients("New Client Connected (IP: " + Clients[index].getIp() + ", name: " + Clients[index].getAccount()->getUserId() + ")");
        }
        else notifyClient(index, "400");
    }
    else {
        // register
        accountDao.registerAccount(id, pw, role);
        shared_ptr<Account> account = accountDao.getAccount(id, pw, role);
        if (account != nullptr) {
            Clients[index].setAccount(account);
            notifyClient(index, "201");
            notifyAllClients("New Client Connected (IP: " + Clients[index].getIp() + ", name: " + Clients[index].getAccount()->getUserId() + ")");
        }
        else notifyClient(index, "401");
    }
}

void ServerCore::handleGetTestItem(const int index) {
    string id = Clients[index].getAccount()->getUserId();
    bool res = inventoryDao.insertItem(id, 1);
    if (res) {
        notifyClient(index, "grant user '" + id + "' test item.");
        cout << "grant user '" + id + "' test item." << endl;
    }
    else{
        notifyClient(index, "error, can't grant user '" + id + "' test item.");
        cout << "error, can't grant user '" + id + "' test item." << endl;
    }
}

void ServerCore::handleInventoryCheck(const int index) {
    string id = Clients[index].getAccount()->getUserId();
    string res = inventoryDao.inventoryCheck(id);
    notifyClient(index, res);
}

void ServerCore::handleSellItem(const int index, const string& msg) {
    string id = Clients[index].getAccount()->getUserId();
    vector<string> params = split(msg, ',');
    bool res = inventoryDao.sellItem(id, stoi(params[0]), stoi(params[1]), stoi(params[2]), stoi(params[3]));
    if (res) notifyClient(index, "Item has been successfully registered in the auction.");
    else notifyClient(index, "Failed to register the item in the auction.");
}

void ServerCore::handleBuyNow(const int index, const string& msg) {
    string id = Clients[index].getAccount()->getUserId();
    vector<string> params = split(msg, ',');
    pair<bool, pair<int, int>> res = inventoryDao.buyNow(id, stoi(params[0]));
    if (res.first) {
        notifyClient(index, "Item has been successfully purchased in the auction.");
        for (int i=1; i<ClientsCount; i++) {
            if (Clients[i].getAccount()->getAccountId() == res.second.first)
                notifyClient(i, "your item has been sold to " + id);
            else if (Clients[i].getAccount()->getAccountId() == res.second.second)
                notifyClient(i, "Your auction has been outbid. please check your inventory");
        }
    }
    else notifyClient(index, "Failed to purchase the item in the auction.");
}

void ServerCore::handleBid(const int index, const string& msg) {
    int account_id = Clients[index].getAccount()->getAccountId();
    vector<string> params = split(msg, ',');
    pair<bool, int> res = inventoryDao.bid(account_id, stoi(params[0]), stoi(params[1]));
    if (res.first) {
        notifyClient(index, "Bid has been successfully placed in the auction.");
        for (int i=1; i<ClientsCount; i++) {
            if (Clients[i].getAccount()->getAccountId() == res.second)
                notifyClient(i, "Your auction has been outbid. please check your inventory");
        }
    }
    else notifyClient(index, "Failed to place the bid in the auction.");
}

void ServerCore::handleBreakItem(const int index, const string& msg) {
    string id = Clients[index].getAccount()->getUserId();
    vector<string> params = split(msg, ',');
    bool res = inventoryDao.breakItem(id, stoi(params[0]), stoi(params[1]));
    if (res) notifyClient(index, "Item has been successfully breaked in the inventory.");
    else notifyClient(index, "Failed to break the item.");
}

void ServerCore::handleOpenPermissionStore(const int index) {
    int account_id = Clients[index].getAccount()->getAccountId();
    string res = permissionStoreDao.openPermissionStore();
    notifyClient(index, res);
}

void ServerCore::handleBuyPermission(const int index, const string& msg) {
    const string& id = Clients[index].getAccount()->getUserId();
    vector<string> params = split(msg, ',');
    bool res = permissionStoreDao.buyPermission(id, stoi(params[0]));
    if (res) notifyClient(index, "Permission has been successfully purchased in the store.");
    else notifyClient(index, "Failed to purchased the permission");
}























































