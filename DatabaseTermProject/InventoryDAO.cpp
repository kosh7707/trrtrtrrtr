#include "InventoryDAO.h"

bool InventoryDAO::insertItem(const int account_id, const int item_id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();

    const char* queryTemplate = "insert into inventory (account_id, item_id, quantity) values ('%d', '%d', '1')\n"
                                "on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
    string query = DatabaseConnection::queryFormatting(queryTemplate, account_id, item_id);
    bool res = databaseConnection.commandQuery(query);
    return res;
}

bool InventoryDAO::deleteItem(const int account_id, const int item_id, const int quantity) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();

    const char* queryTemplate1 = "select * from inventory i where i.account_id = %d and item_id = %d and quantity >= %d;";
    string query1 = DatabaseConnection::queryFormatting(queryTemplate1, account_id, item_id, quantity);
    auto res1 = databaseConnection.selectQuery(query1);
    if (res1.size() == 1) {
        if (quantity == stoi(res1[0]["quantity"])) {
            const char* queryTemplate2 = "delete from inventory i where i.account_id = %d and i.item_id = %d;";
            string query2 = DatabaseConnection::queryFormatting(queryTemplate2, account_id, item_id);
            return databaseConnection.commandQuery(query2);
        }
        else {
            const char* queryTemplate2 = "update inventory set quantity = quantity - %d where account_id = %d and item_id = %d;";
            string query2 = DatabaseConnection::queryFormatting(queryTemplate2, quantity, account_id, item_id);
            return databaseConnection.commandQuery(query2);
        }
    }
    return false;
}

string InventoryDAO::inventoryCheck(const int account_id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    ostringstream ss;

    const char* queryTemplate1 = "select user_id 아이디, balance 잔고 from accounts where account_id = %d;";
    string query1 = DatabaseConnection::queryFormatting(queryTemplate1, account_id);
    auto res1 = databaseConnection.selectQuery(query1);
    ss << "----------------------\n";
    for (const auto& row : res1) {
        for (const auto& p : row)
            ss << p.first << ":" << p.second << "\n";
        ss << "----------------------\n";
    }

    const char* queryTemplate2 = "select items.item_id item_id, items.name 아이템이름, items.score 점수, buffs.name 버프이름, inventory.quantity 수량\n"
                                 "from accounts ac, items, inventory, buffs\n"
                                 "where inventory.account_id = %d\n"
                                 "  and items.item_id = inventory.item_id\n"
                                 "  and items.buff_id = buffs.buff_id\n"
                                 "order by items.item_id;";
    char queryBuffer[1024];
    snprintf(queryBuffer, sizeof(queryBuffer), queryTemplate2, account_id);
    string query(queryBuffer);
    auto res2 = databaseConnection.selectQuery(query);
    ss << "----------------------\n";
    for (const auto& row : res2) {
        for (const auto& p : row)
            ss << p.first << ":" << p.second << "\n";
        ss << "----------------------\n";
    }
    return ss.str();
}

bool InventoryDAO::sellItem(const int account_id, const int item_id, const int quantity, const int buyNowPrice, const int startingBidPrice) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();

    const char* queryTemplate1 = "select * from inventory i where i.account_id = %d and item_id = %d and quantity >= %d;";
    string query1 = DatabaseConnection::queryFormatting(queryTemplate1, account_id, item_id, quantity);
    auto res1 = databaseConnection.selectQuery(query1);
    if (res1.size() != 1) return false;

    vector<string> queries;
    string query2;
    if (quantity == stoi(res1[0]["quantity"])) {
        const char* queryTemplate2 = "delete from inventory i where i.account_id = %d and i.item_id = %d;";
        query2 = DatabaseConnection::queryFormatting(queryTemplate2, account_id, item_id);
    }
    else {
        const char* queryTemplate2 = "update inventory set quantity = quantity - %d where account_id = %d and item_id = %d;";
        query2 = DatabaseConnection::queryFormatting(queryTemplate2, quantity, account_id, item_id);
    }
    const char* queryTemplate3 = "insert into auctions(item_id, item_quantity, seller_id, end_time, current_price, buy_now_price)\n"
                                 "    values (%d, %d, %d, CURRENT_TIMESTAMP + INTERVAL '1 minutes', %d, %d);";
    string query3 = DatabaseConnection::queryFormatting(queryTemplate3, item_id, quantity, account_id, startingBidPrice, buyNowPrice);
    queries.emplace_back(query2);
    queries.emplace_back(query3);
    return databaseConnection.transaction(queries);
}

pair<bool, pair<int, int>> InventoryDAO::buyNow(const int account_id, const int auction_id) {
    IDatabaseConnection &databaseConnection = DatabaseConnection::getInstance();

    auto res = databaseConnection.selectQuery("select * from auctions where auction_id = " + to_string(auction_id) + ";");
    if (res.size() != 1) return pair<bool, pair<int, int>>(false, {-1, -1});

    int balance = stoi(databaseConnection.selectQuery("select balance from accounts where account_id = '" + to_string(account_id) + "';")[0]["balance"]);
    int price = stoi(res[0]["buy_now_price"]);
    int item_id = stoi(res[0]["item_id"]);
    int quantity = stoi(res[0]["item_quantity"]);
    int seller_id = stoi(res[0]["seller_id"]);
    if (balance < price) return pair<bool, pair<int, int>>(false, {-1, -1});

    vector<string> queries;
    if (res[0]["current_bidder_id"] == "") {
        const char *queryTemplate1 = "update accounts set balance = balance - %d where account_id = %d;";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate1, price, account_id));
        const char *queryTemplate2 = "insert into inventory (account_id, item_id, quantity) values (%d, %d, %d)\n"
                                     "on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate2, account_id, item_id, quantity));
        const char *queryTemplate3 = "delete from auctions where auction_id = %d";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate3, auction_id));
        const char *queryTemplate4 = "update accounts set balance = balance + %d where account_id = %d;";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate4, price, seller_id));
        return pair<bool, pair<int, int>>(databaseConnection.transaction(queries), {seller_id, -1});
    }
    else {
        int current_bidder_id = stoi(res[0]["current_bidder_id"]);
        int current_price = stoi(res[0]["current_price"]);
        const char *queryTemplate1 = "update accounts set balance = balance - %d where account_id = %d;";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate1, price, account_id));
        const char *queryTemplate2 = "insert into inventory (account_id, item_id, quantity) values (%d, %d, %d)\n"
                                     "on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate2, account_id, item_id, quantity));
        const char *queryTemplate3 = "delete from auctions where auction_id = %d";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate3, auction_id));
        const char *queryTemplate4 = "update accounts set balance = balance + %d where account_id = %d;";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate4, price, seller_id));
        const char* queryTemplate5 = "update accounts set balance = balance + %d where account_id = %d";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate5, current_price, current_bidder_id));
        return pair<bool, pair<int, int>>(databaseConnection.transaction(queries), {seller_id, current_bidder_id});
    }
}

bool InventoryDAO::breakItem(const int account_id, const int item_id, const int quantity) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();

    const char* queryTemplate1 = "select * from inventory i where i.account_id = %d and item_id = %d and quantity >= %d;";
    string query1 = DatabaseConnection::queryFormatting(queryTemplate1, account_id, item_id, quantity);
    auto res1 = databaseConnection.selectQuery(query1);
    if (res1.size() == 1) {
        vector<string> queries;
        if (quantity == stoi(res1[0]["quantity"])) {
            const char* queryTemplate2 = "delete from inventory i where i.account_id = %d and i.item_id = %d;";
            databaseConnection.commandQuery(DatabaseConnection::queryFormatting(queryTemplate2, account_id, item_id));
        }
        else {
            const char* queryTemplate2 = "update inventory set quantity = quantity - %d where account_id = %d and item_id = %d;";
            databaseConnection.commandQuery(DatabaseConnection::queryFormatting(queryTemplate2, quantity, account_id, item_id));
        }
        return databaseConnection.transaction(queries);
    }
    return false;
}

pair<bool, int> InventoryDAO::bid(const int account_id, const int auction_id, const int price) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();

    auto res = databaseConnection.selectQuery("select * from auctions where auction_id = " + to_string(auction_id) + ";");
    if (res.size() != 1) return pair<bool, int>(false, -1);

    int current_price = stoi(res[0]["current_price"]);
    if (price <= current_price) return pair<bool, int>(false, -1);

    if (res[0]["current_bidder_id"] == "") {
        vector<string> queries;
        const char* queryTemplate1 = "update auctions set current_bidder_id = %d, current_price = %d, end_time = CURRENT_TIMESTAMP + INTERVAL '1 minutes' where auction_id = %d;";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate1, account_id, price, auction_id));
        const char* queryTemplate2 = "update accounts set balance = balance - %d where account_id = %d";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate2, price, account_id));
        return pair<bool, int>(databaseConnection.transaction(queries), -1);
    }
    else {
        int current_bidder_id = stoi(res[0]["current_bidder_id"]);
        vector<string> queries;
        const char* queryTemplate1 = "update auctions set current_bidder_id = %d, current_price = %d, end_time = CURRENT_TIMESTAMP + INTERVAL '1 minutes' where auction_id = %d;";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate1, account_id, price, auction_id));
        const char* queryTemplate2 = "update accounts set balance = balance - %d where account_id = %d";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate2, price, account_id));
        const char* queryTemplate3 = "update accounts set balance = balance + %d where account_id = %d";
        queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate3, current_price, current_bidder_id));
        return pair<bool, int>(databaseConnection.transaction(queries), current_bidder_id);
    }
}


