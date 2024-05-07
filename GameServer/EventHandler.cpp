#include "EventHandler.h"

std::vector<std::unique_ptr<Event>> EventHandler::handling(std::unique_ptr<Event> event) {
    int         index       = event->getIndex();
    int         eventCode   = event->getEventCode();
    std::string contents    = event->getContents();

    std::cout << "index: " << index << ", "
              << "eventCode: " << eventCode << ", "
              << "contents: " << contents << std::endl;

    std::vector<std::unique_ptr<Event>> ret;
    switch (eventCode) {
        case Event::LOGIN_EVENT:
            ret = handleLogin(index, contents);
            break;
        case Event::INVENTORY_CHECK_EVENT:
            ret = handleInventoryCheck(index);
            break;
        case Event::GET_TEST_ITEM_EVENT:
            ret = handleGetTestItem(index, contents);
            break;
        case Event::BREAK_ITEM_EVENT:
            ret = handleBreakItem(index, contents);
            break;
        case Event::USER_INFO_EVENT:
            ret = handleUserInfo(index);
            break;
        default:
            std::cerr << "Invalid Event" << std::endl;
            break;
    }
    return ret;
}

std::vector<std::string> EventHandler::split(const std::string& input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;
    while (getline(ss, token, ','))
        result.push_back(token);
    return result;
}

std::vector<std::unique_ptr<Event>> EventHandler::handleLogin(const int index, const std::string& contents) {
    std::vector<std::unique_ptr<Event>> ret;
    auto params = split(contents);
    std::string id = params[0], pw = params[1];
    if (accountService->isExistAccount(id)) {
        auto account = accountService->loginAccount(id, pw);
        if (account == nullptr) ret.emplace_back(std::make_unique<Event>(index, Event::LOGIN_FAIL, "Incorrect username or password."));
        else {
            ret.emplace_back(std::make_unique<Event>(index, Event::LOGIN_SUCCESS, "Successfully logged in. Welcome " + id + "."));
            auto inventory = inventoryService->getInventory(account->getAccountId());
            SOCKET sc = observer->indexToSocket[index].getSc();
            observer->socketToClient[sc].setAccount(std::move(account));
            observer->socketToClient[sc].setInventory(std::move(inventory));
        }
    }
    else {
        auto account = accountService->registerAccount(id, pw);
        if (account == nullptr) ret.emplace_back(std::make_unique<Event>(index, Event::REGISTER_FAIL, "Registration failed."));
        else {
            ret.emplace_back(std::make_unique<Event>(index, Event::LOGIN_SUCCESS, "Successfully registered. Welcome " + id + "."));
            auto inventory = inventoryService->getInventory(account->getAccountId());
            SOCKET sc = observer->indexToSocket[index].getSc();
            observer->socketToClient[sc].setAccount(std::move(account));
            observer->socketToClient[sc].setInventory(std::move(inventory));
        }
    }
    return ret;
}

std::vector<std::unique_ptr<Event>> EventHandler::handleInventoryCheck(const int index) {
    std::vector<std::unique_ptr<Event>> ret;

    SOCKET sc = observer->indexToSocket[index].getSc();
    auto items = observer->socketToClient[sc].getInventory()->getItems();
    std::stringstream ss;
    for (auto item : *items) {
        ss << item.second.getItemId()   << ","
           << item.second.getScore()    << ","
           << item.second.getQuantity() << "|";
    }
    ret.emplace_back(std::make_unique<Event>(index, Event::CHAT_EVENT, ss.str()));
    return ret;
}

std::vector<std::unique_ptr<Event>> EventHandler::handleGetTestItem(const int index, const std::string& contents) {
    std::vector<std::unique_ptr<Event>> ret;

    SOCKET sc = observer->indexToSocket[index].getSc();
    auto params = split(contents);
    int item_id = std::stoi(params[0]); int quantity = std::stoi(params[1]);
    bool res = inventoryService->grantItem(observer->socketToClient[sc].getAccount(), observer->socketToClient[sc].getInventory(), item_id, quantity);
    if (res) ret.emplace_back(std::make_unique<Event>(index, Event::CHAT_EVENT, "Get test item, item_id: " + std::to_string(item_id) + ", quantity: " + std::to_string(quantity)));
    else ret.emplace_back(std::make_unique<Event>(index, Event::CHAT_EVENT, "Fail to get test item."));
    return ret;
}

std::vector<std::unique_ptr<Event>> EventHandler::handleBreakItem(const int index, const std::string& contents) {
    std::vector<std::unique_ptr<Event>> ret;

    SOCKET sc = observer->indexToSocket[index].getSc();
    auto params = split(contents);
    int item_id = std::stoi(params[0]); int quantity = std::stoi(params[1]);
    bool res = inventoryService->breakItem(observer->socketToClient[sc].getAccount(), observer->socketToClient[sc].getInventory(), item_id, quantity);
    if (res) ret.emplace_back(std::make_unique<Event>(index, Event::CHAT_EVENT, "successfully break the item."));
    else ret.emplace_back(std::make_unique<Event>(index, Event::CHAT_EVENT, "Fail to break the item."));
    return ret;
}

std::vector<std::unique_ptr<Event>> EventHandler::handleUserInfo(const int index) {
    std::vector<std::unique_ptr<Event>> ret;

    SOCKET sc = observer->indexToSocket[index].getSc();
    auto account = observer->socketToClient[sc].getAccount().get();
    std::string user_id = account->getUserId();
    int score = account->getScore();
    int balance = account->getBalance();
    std::string contents = user_id + "," + std::to_string(score) + "," + std::to_string(balance);
    ret.emplace_back(std::make_unique<Event>(index, Event::CHAT_EVENT, contents));
    return ret;
}

//std::vector<std::pair<int, std::string>> EventHandler::handleSellItem(const int index, const std::string& msg, const std::unique_ptr<Client[]>& Clients) {
//    std::vector<std::pair<int, std::string>> ret;
//    int account_id = Clients[index].getAccount()->getAccountId();
//    std::vector<std::string> params = split(msg);
//    bool res = inventoryDao.sellItem(account_id, stoi(params[0]), stoi(params[1]), stoi(params[2]), stoi(params[3]));
//    if (res) ret.push_back({index, "[1]Item has been successfully registered in the auction."});
//    else ret.push_back({index, "[1]Failed to register the item in the auction."});
//    return ret;
//}
//
//std::vector<std::pair<int, std::string>> EventHandler::handleBuyNow(const int index, const std::string& msg, const int ClientsCount, const std::unique_ptr<Client[]>& Clients) {
//    std::vector<std::pair<int, std::string>> ret;
//    int account_id = Clients[index].getAccount()->getAccountId();
//    std::vector<std::string> params = split(msg);
//    std::pair<bool, std::pair<int, int>> res = inventoryDao.buyNow(account_id, stoi(params[0]));
//    if (res.first) {
//        ret.push_back({index, "[1]Item has been successfully purchased in the auction."});
//        for (int i=1; i<ClientsCount; i++) {
//            if (Clients[i].getAccount()->getAccountId() == res.second.first)
//                ret.push_back({i, "[1]your item has been sold to " + Clients[i].getAccount()->getUserId()});
//            else if (Clients[i].getAccount()->getAccountId() == res.second.second)
//                ret.push_back({i, "[1]Your auction has been outbid. please check your inventory"});
//        }
//    }
//    else ret.push_back({index, "[1]Failed to purchase the item in the auction."});
//    return ret;
//}
//
//std::vector<std::pair<int, std::string>> EventHandler::handleBid(const int index, const std::string& msg, const int ClientsCount, const std::unique_ptr<Client[]>& Clients) {
//    std::vector<std::pair<int, std::string>> ret;
//    int account_id = Clients[index].getAccount()->getAccountId();
//    std::vector<std::string> params = split(msg);
//    std::pair<bool, int> res = inventoryDao.bid(account_id, stoi(params[0]), stoi(params[1]));
//    if (res.first) {
//        ret.push_back({index, "[1]Bid has been successfully placed in the auction."});
//        for (int i=1; i<ClientsCount; i++) {
//            if (Clients[i].getAccount()->getAccountId() == res.second)
//                ret.push_back({i, "[1]Your auction has been outbid. please check your inventory"});
//        }
//    }
//    else ret.push_back({index, "[1]Failed to place the bid in the auction."});
//    return ret;
//}
//

//std::vector<std::pair<int, std::string>> EventHandler::handleAuctionCheck(const int index) {
//    std::vector<std::pair<int, std::string>> ret;
//    std::string res = auctionDao.auctionCheck();
//    ret.push_back({index, "[1]" + res});
//    return ret;
//}


