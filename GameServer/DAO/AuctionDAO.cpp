//#include "AuctionDAO.h"
//
//std::string AuctionDAO::auctionCheck() {
//    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
//    std::stringstream ss;
//
//    auto res1 = databaseConnection.selectQuery("select * from auctions");
//    ss << "----------------------\n";
//    for (const auto& row : res1) {
//        for (const auto& p : row)
//            ss << p.first << ":" << p.second << "\n";
//        ss << "----------------------\n";
//    }
//    return ss.str();
//}
//
//std::vector<std::pair<int, std::string>> AuctionDAO::outdatedItemCheck(int ClientsCount, const std::unique_ptr<Client[]>& Clients) {
//    std::vector<std::pair<int, std::string>> ret;
//
//    IDatabaseConnection& dc = DatabaseConnection::getInstance();
//    auto res = dc.selectQuery("select * from auctions where end_time <= CURRENT_TIMESTAMP");
//    for (auto row : res) {
//        std::vector<std::string> queries;
//        int auction_id = stoi(row["auction_id"]);
//        int item_id = stoi(row["item_id"]);
//        int seller_id = stoi(row["seller_id"]);
//        int item_quantity = stoi(row["item_quantity"]);
//        if (row["current_bidder_id"] != "") {
//            int current_bidder_id = stoi(row["current_bidder_id"]);
//            int current_price = stoi(row["current_price"]);
//            const char* queryTemplate1 = "delete from auctions where auction_id = %d;";
//            queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate1, auction_id));
//            const char* queryTemplate2 = "update accounts set balance = balance + %d where account_id = %d;";
//            queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate2, current_price, seller_id));
//            const char* queryTemplate3 = "insert into inventory (account_id, item_id, quantity) values (%d, %d, %d)\n"
//                                         "on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
//            queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate3, current_bidder_id, item_id, item_quantity));
//            if (dc.transaction(queries)) {
//                for (int i=1; i<ClientsCount; i++) {
//                    if (Clients[i].getAccount()->getAccountId() == current_bidder_id)
//                        ret.push_back({i, "[1]You've won the auction item_id: " + std::to_string(item_id) + ". please check your inventory"});
//                    else if (Clients[i].getAccount()->getAccountId() == seller_id)
//                        ret.push_back({i, "[1]your item has been sold out. please check your inventory"});
//                }
//            }
//        }
//        else {
//            const char* queryTemplate1 = "delete from auctions where auction_id = %d;";
//            queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate1, auction_id));
//            const char* queryTemplate2 = "insert into inventory (account_id, item_id, quantity) values (%d, %d, %d)\n"
//                                         "on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
//            queries.emplace_back(DatabaseConnection::queryFormatting(queryTemplate2, seller_id, item_id, item_quantity));
//            if (dc.transaction(queries)) {
//                for (int i=1; i<ClientsCount; i++) {
//                    if (Clients[i].getAccount()->getAccountId() == seller_id)
//                        ret.push_back({i, "[1]your auction has been expired. please check your inventory"});
//                }
//            }
//        }
//    }
//    return ret;
//}
