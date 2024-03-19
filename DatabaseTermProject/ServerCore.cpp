#include "ServerCore.h"
#include "EventHandler.h"

ServerCore::ServerCore() :  maxClientCount(serverConstant.getMaxClientCount()),
                            serverPort(serverConstant.getServerPort()),
                            serverIP(serverConstant.getServerIP()) {
    eventHandler = new EventHandler(this);
    Clients = std::shared_ptr<Client[]>(new Client[maxClientCount + 1]);
    ClientsCount = 0;
}

SOCKET ServerCore::initServer() {
    WSADATA wsadata;
    SOCKET sc;
    SOCKADDR_IN socketAddress;

    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsadata) != 0) {
        std::cout << "WSAStartup Error." << std::endl;
        return 0;
    }

    if ((sc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cout << "socket creation error." << std::endl;
        return 0;
    }

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    socketAddress.sin_port = htons(serverPort);
    if (bind(sc, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(socketAddress)) < 0) {
        std::cout << "bind error." << "\n";
        return 0;
    }

    if (listen(sc, SOMAXCONN) < 0) {
        std::cout << "listen error." << std::endl;
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
        std::cout << "socket initialization error" << std::endl;
        exit(0);
    }

    HANDLE event = WSACreateEvent();
    Clients[ClientsCount].setEv(event);
    Clients[ClientsCount].setSc(sc);
    Clients[ClientsCount].setIp("0.0.0.0");

    WSAEventSelect(sc, event, FD_ACCEPT);
    ClientsCount++;

    std::cout << "server listening at 127.0.0.1:7707" << std::endl;
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

void ServerCore::readClient(const int index) {
    char buf[BUF_SIZE + 1];
    recv(Clients[index].getSc(), buf, BUF_SIZE, 0);
    std::cout << "[Recv] Clients[" << index << "]: " << buf << "\n";
    eventHandler->handling(index, buf);
}

void ServerCore::removeClient(const int index) {
    if (Clients[index].getAccount() == nullptr) {
        ClientsCount--;
        std::swap(Clients[index], Clients[ClientsCount]);
    }
    else {
        std::string removeClientIP = Clients[index].getIp();
        std::string removeClientNickName = Clients[index].getAccount()->getUserId();
        ClientsCount--;
        std::swap(Clients[index], Clients[ClientsCount]);
        notifyAllClients("Client Disconnected (IP: " + removeClientIP + ", name: " + removeClientNickName + ")");
    }
}

void ServerCore::notifyAllClients(const std::string& msg) {
    for (int i=1; i<ClientsCount; i++)
        send(Clients[i].getSc(), msg.c_str(), BUF_SIZE, 0);
}

void ServerCore::notifyClient(const int index, const std::string& msg) {
    send(Clients[index].getSc(), msg.c_str(), BUF_SIZE, 0);
}

unsigned int WINAPI ServerCore::runAuctionWorkerThread(void* params) {
    ServerCore* serverCore = static_cast<ServerCore*>(params);
    IDatabaseConnection& dc = DatabaseConnection::getInstance();
    while (dc.isConnected()) {
        auto res = dc.selectQuery("select * from auctions where end_time <= CURRENT_TIMESTAMP");
        for (auto row : res) {
            std::vector<std::string> queries;
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
                            serverCore->notifyClient(i, "You've won the auction item_id: " + std::to_string(item_id) + ". please check your inventory");
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
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return 0;
}

void ServerCore::runAuctionWorker() {
    unsigned int tid;
    auctionWorker = (HANDLE)_beginthreadex(NULL, 0, runAuctionWorkerThread, (void*)this, 0, &tid);
}
