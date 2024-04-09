#include "EventHandler.h"

std::vector<std::pair<int, std::string>> EventHandler::handling(const int index, const std::string& buf, const int ClientsCount, const std::unique_ptr<Client[]>& Clients) {
    int event = getEvent(buf); std::string msg = getMessage(buf);
    std::vector<std::pair<int, std::string>> ret;
    if (event == LOGIN_EVENT) ret = handleLogin(index, msg, Clients);
    else if (event == CHAT_EVENT) ret = handleChat(index, msg, Clients);
    else if (event == GET_TEST_ITEM_EVENT) ret = handleGetTestItem(index, Clients);
    else if (event == INVENTORY_CHECK_EVENT) ret = handleInventoryCheck(index, Clients);
    else if (event == SELL_ITEM_EVENT) ret = handleSellItem(index, msg, Clients);
    else if (event == BUY_NOW_EVENT) ret = handleBuyNow(index, msg, ClientsCount, Clients);
    else if (event == BID_EVENT) ret = handleBid(index, msg, ClientsCount, Clients);
    else if (event == BREAK_ITEM_EVENT) ret = handleBreakItem(index, msg, Clients);
    else if (event == AUCTION_CHECK_EVENT) ret = handleAuctionCheck(index);
    else std::cout << "INVALID EVENT\nmsg: " << msg << std::endl;
    return ret;
}

std::vector<std::string> EventHandler::split(const std::string &input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;
    while (getline(ss, token, ','))
        result.push_back(token);
    return result;
}

int EventHandler::getEvent(const std::string& buf) {
    std::string prefix = buf.substr(0, 3);
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

std::string EventHandler::getMessage(const std::string& buf) {
    std::string temp = buf;
    return temp.substr(3);
}

std::vector<std::pair<int, std::string>> EventHandler::handleLogin(const int index, const std::string& msg, const std::unique_ptr<Client[]>& Clients) {
    std::vector<std::pair<int, std::string>> ret;
    std::vector<std::string> params = split(msg);
    std::string id = params[0], pw = params[1];
    if (accountDao.checkAccountExists(id)) {
        std::unique_ptr<Account> account = std::move(accountDao.getAccount(id, pw));
        if (account != nullptr) {
            Clients[index].setAccount(std::move(account));
            accountDao.updateAccountLastLogin(id);
            ret.push_back({index, "[0]000"});
            ret.push_back({0, "[1]New Client Connected (IP: " + Clients[index].getGameSocket()->getIp() + ", name: " + Clients[index].getAccount()->getUserId() + ")"});
        }
        else ret.push_back({index, "[0]002"});
    }
    else {
        accountDao.registerAccount(id, pw);
        std::unique_ptr<Account> account = accountDao.getAccount(id, pw);
        if (account != nullptr) {
            Clients[index].setAccount(std::move(account));
            ret.push_back({index, "[0]001"});
            ret.push_back({0, "[1]New Client Connected (IP: " + Clients[index].getGameSocket()->getIp() + ", name: " + Clients[index].getAccount()->getUserId() + ")"});
        }
        else ret.push_back({index, "[0]003"});
    }
    return ret;
}

std::vector<std::pair<int, std::string>> EventHandler::handleChat(const int index, const std::string& msg, const std::unique_ptr<Client[]>& Clients) {
    std::vector<std::pair<int, std::string>> ret;
    ret.push_back({0, "[1]" + Clients[index].getAccount()->getUserId() + " : " + msg});
    return ret;
}

std::vector<std::pair<int, std::string>> EventHandler::handleGetTestItem(const int index, const std::unique_ptr<Client[]>& Clients) {
    std::vector<std::pair<int, std::string>> ret;
    int account_id = Clients[index].getAccount()->getAccountId();
    bool res = inventoryDao.insertItem(account_id, 1);
    if (res) {
        ret.push_back({index, "[1]Give you test item."});
        std::cout << "grant user '" + std::to_string(account_id) + "' test item." << std::endl;
    }
    else{
        ret.push_back({index, "[1]error , can't give you test item."});
        std::cout << "error, can't grant user '" + std::to_string(account_id) + "' test item." << std::endl;
    }
    return ret;
}

std::vector<std::pair<int, std::string>> EventHandler::handleInventoryCheck(const int index, const std::unique_ptr<Client[]>& Clients) {
    std::vector<std::pair<int, std::string>> ret;
    int account_id = Clients[index].getAccount()->getAccountId();
    std::string res = inventoryDao.inventoryCheck(account_id);
    ret.push_back({index, "[1]" + res});
    return ret;
}

std::vector<std::pair<int, std::string>> EventHandler::handleSellItem(const int index, const std::string& msg, const std::unique_ptr<Client[]>& Clients) {
    std::vector<std::pair<int, std::string>> ret;
    int account_id = Clients[index].getAccount()->getAccountId();
    std::vector<std::string> params = split(msg);
    bool res = inventoryDao.sellItem(account_id, stoi(params[0]), stoi(params[1]), stoi(params[2]), stoi(params[3]));
    if (res) ret.push_back({index, "[1]Item has been successfully registered in the auction."});
    else ret.push_back({index, "[1]Failed to register the item in the auction."});
    return ret;
}

std::vector<std::pair<int, std::string>> EventHandler::handleBuyNow(const int index, const std::string& msg, const int ClientsCount, const std::unique_ptr<Client[]>& Clients) {
    std::vector<std::pair<int, std::string>> ret;
    int account_id = Clients[index].getAccount()->getAccountId();
    std::vector<std::string> params = split(msg);
    std::pair<bool, std::pair<int, int>> res = inventoryDao.buyNow(account_id, stoi(params[0]));
    if (res.first) {
        ret.push_back({index, "[1]Item has been successfully purchased in the auction."});
        for (int i=1; i<ClientsCount; i++) {
            if (Clients[i].getAccount()->getAccountId() == res.second.first)
                ret.push_back({i, "[1]your item has been sold to " + Clients[i].getAccount()->getUserId()});
            else if (Clients[i].getAccount()->getAccountId() == res.second.second)
                ret.push_back({i, "[1]Your auction has been outbid. please check your inventory"});
        }
    }
    else ret.push_back({index, "[1]Failed to purchase the item in the auction."});
    return ret;
}

std::vector<std::pair<int, std::string>> EventHandler::handleBid(const int index, const std::string& msg, const int ClientsCount, const std::unique_ptr<Client[]>& Clients) {
    std::vector<std::pair<int, std::string>> ret;
    int account_id = Clients[index].getAccount()->getAccountId();
    std::vector<std::string> params = split(msg);
    std::pair<bool, int> res = inventoryDao.bid(account_id, stoi(params[0]), stoi(params[1]));
    if (res.first) {
        ret.push_back({index, "[1]Bid has been successfully placed in the auction."});
        for (int i=1; i<ClientsCount; i++) {
            if (Clients[i].getAccount()->getAccountId() == res.second)
                ret.push_back({i, "[1]Your auction has been outbid. please check your inventory"});
        }
    }
    else ret.push_back({index, "[1]Failed to place the bid in the auction."});
    return ret;
}

std::vector<std::pair<int, std::string>> EventHandler::handleBreakItem(const int index, const std::string& msg, const std::unique_ptr<Client[]>& Clients) {
    std::vector<std::pair<int, std::string>> ret;
    int account_id = Clients[index].getAccount()->getAccountId();
    std::vector<std::string> params = split(msg);
    bool res = inventoryDao.breakItem(account_id, stoi(params[0]), stoi(params[1]));
    if (res) ret.push_back({index, "[1]Item has been successfully breaked in the inventory."});
    else ret.push_back({index, "[1]Failed to break the item."});
    return ret;
}

std::vector<std::pair<int, std::string>> EventHandler::handleAuctionCheck(const int index) {
    std::vector<std::pair<int, std::string>> ret;
    std::string res = auctionDao.auctionCheck();
    ret.push_back({index, "[1]" + res});
    return ret;
}