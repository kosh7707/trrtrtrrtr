#include "EventHandler.h"
#include "ServerCore.h"

EventHandler::EventHandler(ServerCore* serverCore) : serverCore(serverCore) {}

void EventHandler::handling(const int index, const char* buf) {
    int event = getEvent(buf); std::string msg = getMessage(buf);
    if (event == LOGIN_EVENT) handleLogin(index, msg);
    else if (event == CHAT_EVENT) handleChat(index, msg);
    else if (event == GET_TEST_ITEM_EVENT) handleGetTestItem(index);
    else if (event == INVENTORY_CHECK_EVENT) handleInventoryCheck(index);
    else if (event == SELL_ITEM_EVENT) handleSellItem(index, msg);
    else if (event == BUY_NOW_EVENT) handleBuyNow(index, msg);
    else if (event == BID_EVENT) handleBid(index, msg);
    else if (event == BREAK_ITEM_EVENT) handleBreakItem(index, msg);
    else if (event == AUCTION_CHECK_EVENT) handleAuction(index);
    else std::cout << "INVALID EVENT\nmsg: " << msg << std::endl;
}

std::vector<std::string> EventHandler::split(const std::string &input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;
    while (getline(ss, token, ','))
        result.push_back(token);
    return result;
}

int EventHandler::getEvent(const char* buf) {
    std::string temp = buf;
    std::string prefix = temp.substr(0, 3);
    if (prefix == "[0]") return LOGIN_EVENT;
    else if (prefix == "[1]") return CHAT_EVENT;
    else if (prefix == "[2]") return GET_TEST_ITEM_EVENT;
    else if (prefix == "[3]") return INVENTORY_CHECK_EVENT;
    else if (prefix == "[4]") return SELL_ITEM_EVENT;
    else if (prefix == "[5]") return BUY_NOW_EVENT;
    else if (prefix == "[6]") return BID_EVENT;
    else if (prefix == "[7]") return BREAK_ITEM_EVENT;
    else if (prefix == "[8]") return AUCTION_CHECK_EVENT;
    else return INVALID_EVENT;
}

std::string EventHandler::getMessage(const char* buf) {
    std::string temp = buf;
    return temp.substr(3);
}

void EventHandler::notifyClient(const int index, const std::string& msg, const int event=CHAT_EVENT) {
    switch (event) {
        case LOGIN_EVENT:
            serverCore->notifyClient(index, "[0]" + msg);
            break;
        case CHAT_EVENT:
            serverCore->notifyClient(index, "[1]" + msg);
            break;
    }
}

void EventHandler::notifyAllClients(const std::string& msg) {
    serverCore->notifyAllClients("[1]" + msg);
}

void EventHandler::handleLogin(const int index, const std::string& msg) {
    std::vector<std::string> params = split(msg);
    std::string id = params[0], pw = params[1];
    if (accountDao.checkAccountExists(id)) {
        // login
        std::shared_ptr<Account> account = accountDao.getAccount(id, pw);
        if (account != nullptr) {
            serverCore->Clients[index].setAccount(account);
            accountDao.updateAccountLastLogin(id);
            notifyClient(index, "000", LOGIN_EVENT);
            notifyAllClients("New Client Connected (IP: " + serverCore->Clients[index].getIp() + ", name: " + serverCore->Clients[index].getAccount()->getUserId() + ")");
        }
        else notifyClient(index, "002", LOGIN_EVENT);
    }
    else {
        // register
        accountDao.registerAccount(id, pw);
        std::shared_ptr<Account> account = accountDao.getAccount(id, pw);
        if (account != nullptr) {
            serverCore->Clients[index].setAccount(account);
            notifyClient(index, "001", LOGIN_EVENT);
            notifyAllClients("New Client Connected (IP: " + serverCore->Clients[index].getIp() + ", name: " + serverCore->Clients[index].getAccount()->getUserId() + ")");
        }
        else notifyClient(index, "003", LOGIN_EVENT);
    }
}

void EventHandler::handleChat(const int index, const std::string& msg) {
    notifyAllClients(serverCore->Clients[index].getAccount()->getUserId() + " : " + msg);
}

void EventHandler::handleGetTestItem(const int index) {
    int account_id = serverCore->Clients[index].getAccount()->getAccountId();
    bool res = inventoryDao.insertItem(account_id, 1);
    if (res) {
        notifyClient(index, "Give you test item.");
        std::cout << "grant user '" + std::to_string(account_id) + "' test item." << std::endl;
    }
    else{
        notifyClient(index, "error, can't give you test item.");
        std::cout << "error, can't grant user '" + std::to_string(account_id) + "' test item." << std::endl;
    }
}

void EventHandler::handleInventoryCheck(const int index) {
    int account_id = serverCore->Clients[index].getAccount()->getAccountId();
    std::string res = inventoryDao.inventoryCheck(account_id);
    notifyClient(index, res);
}

void EventHandler::handleSellItem(const int index, const std::string& msg) {
    int account_id = serverCore->Clients[index].getAccount()->getAccountId();
    std::vector<std::string> params = split(msg);
    bool res = inventoryDao.sellItem(account_id, stoi(params[0]), stoi(params[1]), stoi(params[2]), stoi(params[3]));
    if (res) notifyClient(index, "Item has been successfully registered in the auction.");
    else notifyClient(index, "Failed to register the item in the auction.");
}

void EventHandler::handleBuyNow(const int index, const std::string& msg) {
    int account_id = serverCore->Clients[index].getAccount()->getAccountId();
    std::vector<std::string> params = split(msg);
    std::pair<bool, std::pair<int, int>> res = inventoryDao.buyNow(account_id, stoi(params[0]));
    if (res.first) {
        notifyClient(index, "Item has been successfully purchased in the auction.");
        for (int i=1; i<serverCore->ClientsCount; i++) {
            if (serverCore->Clients[i].getAccount()->getAccountId() == res.second.first)
                notifyClient(i, "your item has been sold to " + serverCore->Clients[i].getAccount()->getUserId());
            else if (serverCore->Clients[i].getAccount()->getAccountId() == res.second.second)
                notifyClient(i, "Your auction has been outbid. please check your inventory");
        }
    }
    else notifyClient(index, "Failed to purchase the item in the auction.");
}

void EventHandler::handleBid(const int index, const std::string& msg) {
    int account_id = serverCore->Clients[index].getAccount()->getAccountId();
    std::vector<std::string> params = split(msg);
    std::pair<bool, int> res = inventoryDao.bid(account_id, stoi(params[0]), stoi(params[1]));
    if (res.first) {
        notifyClient(index, "Bid has been successfully placed in the auction.");
        for (int i=1; i<serverCore->ClientsCount; i++) {
            if (serverCore->Clients[i].getAccount()->getAccountId() == res.second)
                notifyClient(i, "Your auction has been outbid. please check your inventory");
        }
    }
    else notifyClient(index, "Failed to place the bid in the auction.");
}

void EventHandler::handleBreakItem(const int index, const std::string& msg) {
    int account_id = serverCore->Clients[index].getAccount()->getAccountId();
    std::vector<std::string> params = split(msg);
    bool res = inventoryDao.breakItem(account_id, stoi(params[0]), stoi(params[1]));
    if (res) notifyClient(index, "Item has been successfully breaked in the inventory.");
    else notifyClient(index, "Failed to break the item.");
}

void EventHandler::handleAuction(const int index) {
    std::string res = auctionDao.auctionCheck();
    notifyClient(index, res);
}