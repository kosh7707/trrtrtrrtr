#include "AuctionDAO.h"


using namespace AuctionSystem;
std::unique_ptr<std::vector<Auction>> AuctionDAO::readAuctions() {
    auto ret = std::make_unique<std::vector<Auction>>();
    auto rows = db->selectQuery("select * from auctions");
    for (const auto& row : *rows) {
        Auction auction;
        auction.auction_id = stoi(row.at("auction_id"));
        auction.item_id = stoi(row.at("item_id"));
        auction.item_quantity = stoi(row.at("item_quantity"));
        auction.seller_id = stoi(row.at("seller_id"));
        auction.start_time = row.at("start_time");
        auction.end_time = row.at("end_time");
        auction.current_price = stoi(row.at("current_price"));
        auction.current_bidder_id = (row.at("current_bidder_id").empty()) ? -1 : stoi(row.at("current_bidder_id"));
        auction.buy_now_price = stoi(row.at("buy_now_price"));
        ret->emplace_back(auction);
    }
    return std::move(ret);
}

bool AuctionDAO::addAuction(const int account_id, const int item_id, const int quantity, const int buyNowPrice, const int startingBidPrice) {
    const char* queryTemplate1 = "select * from inventory i where i.account_id = %d and item_id = %d and quantity >= %d;";
    std::string query1 = db->queryFormatting(queryTemplate1, account_id, item_id, quantity);
    auto res1 = db->selectQuery(query1);
    if (res1->size() != 1) return false;

    std::string query2;
    if (quantity == stoi((*res1)[0]["quantity"])) {
        const char* queryTemplate2 = "delete from inventory i where i.account_id = %d and i.item_id = %d;";
        query2 = db->queryFormatting(queryTemplate2, account_id, item_id);
    }
    else {
        const char* queryTemplate2 = "update inventory set quantity = quantity - %d where account_id = %d and item_id = %d;";
        query2 = db->queryFormatting(queryTemplate2, quantity, account_id, item_id);
    }
    const char* queryTemplate3 = "insert into auctions(item_id, item_quantity, seller_id, end_time, current_price, buy_now_price) values (%d, %d, %d, CURRENT_TIMESTAMP + INTERVAL '1 minutes', %d, %d);";
    std::string query3 = db->queryFormatting(queryTemplate3, item_id, quantity, account_id, startingBidPrice, buyNowPrice);
    return db->transaction({query2, query3});
}

BidResult AuctionDAO::bid(const int account_id, const int auction_id, const int price) {
    BidResult bidResult;

    auto res = db->selectQuery("select * from auctions where auction_id = " + std::to_string(auction_id) + ";");
    if (res->size() != 1) {
        bidResult.success = false;
        return bidResult;
    }

    int current_price = stoi((*res)[0]["current_price"]);
    if (price <= current_price) {
        bidResult.success = false;
        return bidResult;
    }

    if ((*res)[0]["current_bidder_id"] == "") {
        const char* queryTemplate1 = "update auctions set current_bidder_id = %d, current_price = %d, end_time = CURRENT_TIMESTAMP + INTERVAL '1 minutes' where auction_id = %d;";
        const char* queryTemplate2 = "update accounts set balance = balance - %d where account_id = %d";
        bool ret = db->transaction({
                                            db->queryFormatting(queryTemplate1, account_id, price, auction_id),
                                            db->queryFormatting(queryTemplate2, price, account_id)
                                    });
        bidResult.success = ret;
        bidResult.prev_bidder_id = -1;
        return bidResult;
    }
    else {
        int current_bidder_id = stoi((*res)[0]["current_bidder_id"]);
        const char* queryTemplate1 = "update auctions set current_bidder_id = %d, current_price = %d, end_time = CURRENT_TIMESTAMP + INTERVAL '1 minutes' where auction_id = %d;";
        const char* queryTemplate2 = "update accounts set balance = balance - %d where account_id = %d";
        const char* queryTemplate3 = "update accounts set balance = balance + %d where account_id = %d";
        bool ret = db->transaction({
                                            db->queryFormatting(queryTemplate1, account_id, price, auction_id),
                                            db->queryFormatting(queryTemplate2, price, account_id),
                                            db->queryFormatting(queryTemplate3, current_price, current_bidder_id)
                                    });
        bidResult.success = ret;
        bidResult.prev_bidder_id = current_bidder_id;
        bidResult.prev_price = current_price;
        return bidResult;
    }
}

BuyNowResult AuctionDAO::buyNow(const int account_id, const int auction_id) {
    BuyNowResult buyNowResult;

    auto res = db->selectQuery("select * from auctions where auction_id = " + std::to_string(auction_id) + ";");
    if (res->size() != 1) {
        buyNowResult.success = false;
        return buyNowResult;
    }

    int balance     = stoi((*db->selectQuery("select balance from accounts where account_id = '" + std::to_string(account_id) + "';"))[0]["balance"]);
    int price       = stoi((*res)[0]["buy_now_price"]);
    int item_id     = stoi((*res)[0]["item_id"]);
    int quantity    = stoi((*res)[0]["item_quantity"]);
    int seller_id   = stoi((*res)[0]["seller_id"]);
    if (balance < price) {
        buyNowResult.success = false;
        return buyNowResult;
    }

    if ((*res)[0]["current_bidder_id"] == "") {
        const char *queryTemplate1 = "update accounts set balance = balance - %d where account_id = %d;";
        const char *queryTemplate2 = "insert into inventory (account_id, item_id, quantity) values (%d, %d, %d) on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
        const char *queryTemplate3 = "delete from auctions where auction_id = %d";
        const char *queryTemplate4 = "update accounts set balance = balance + %d where account_id = %d;";
        bool ret = db->transaction({
                                           db->queryFormatting(queryTemplate1, price, account_id),
                                           db->queryFormatting(queryTemplate2, account_id, item_id, quantity),
                                           db->queryFormatting(queryTemplate3, auction_id),
                                           db->queryFormatting(queryTemplate4, price, seller_id)
                                   });
        buyNowResult.success = ret;
        buyNowResult.seller_id = seller_id;
        buyNowResult.item_id = item_id;
        buyNowResult.quantity = quantity;
        buyNowResult.price = price;
        buyNowResult.prev_bidder_id = -1;
        return buyNowResult;
    }
    else {
        int current_bidder_id   = stoi((*res)[0]["current_bidder_id"]);
        int current_price       = stoi((*res)[0]["current_price"]);
        const char *queryTemplate1 = "update accounts set balance = balance - %d where account_id = %d;";
        const char *queryTemplate2 = "insert into inventory (account_id, item_id, quantity) values (%d, %d, %d) on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
        const char *queryTemplate3 = "delete from auctions where auction_id = %d";
        const char *queryTemplate4 = "update accounts set balance = balance + %d where account_id = %d;";
        const char* queryTemplate5 = "update accounts set balance = balance + %d where account_id = %d";
        bool ret = db->transaction({
                                           db->queryFormatting(queryTemplate1, price, account_id),
                                           db->queryFormatting(queryTemplate2, account_id, item_id, quantity),
                                           db->queryFormatting(queryTemplate3, auction_id),
                                           db->queryFormatting(queryTemplate4, price, seller_id),
                                           db->queryFormatting(queryTemplate5, current_price, current_bidder_id)
                                   });
        buyNowResult.success = ret;
        buyNowResult.seller_id = seller_id;
        buyNowResult.item_id = item_id;
        buyNowResult.quantity = quantity;
        buyNowResult.price = price;
        buyNowResult.prev_bidder_id = current_bidder_id;
        buyNowResult.prev_price = current_price;
        return buyNowResult;
    }
}

//std::unique_ptr<std::vector<std::pair<bool, std::pair<int, int>>>> AuctionDAO::outdatedItemCheck() {
//    auto ret = std::make_unique<std::vector<std::pair<bool, std::pair<int, int>>>>();
//    auto res = db->selectQuery("select * from auctions where end_time <= CURRENT_TIMESTAMP");
//    for (const auto& row : *res) {
//        int auction_id      = stoi(row.at("auction_id"));
//        int item_id         = stoi(row.at("item_id"));
//        int seller_id       = stoi(row.at("seller_id"));
//        int item_quantity   = stoi(row.at("item_quantity"));
//        if (row.at("current_bidder_id").empty()) {
//            const char* queryTemplate1 = "delete from auctions where auction_id = %d;";
//            const char* queryTemplate2 = "insert into inventory (account_id, item_id, quantity) values (%d, %d, %d) on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
//            bool txResult = db->transaction({
//                                                    db->queryFormatting(queryTemplate1, auction_id),
//                                                    db->queryFormatting(queryTemplate2, seller_id, item_id, item_quantity)
//                                            });
//            if (txResult) ret->emplace_back(std::pair<bool, std::pair<int, int>>(false, {seller_id, -1}));
//        }
//        else {
//            int current_bidder_id   = stoi(row.at("current_bidder_id"));
//            int current_price       = stoi(row.at("current_price"));
//            const char* queryTemplate1 = "delete from auctions where auction_id = %d;";
//            const char* queryTemplate2 = "update accounts set balance = balance + %d where account_id = %d;";
//            const char* queryTemplate3 = "insert into inventory (account_id, item_id, quantity) values (%d, %d, %d) on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
//            bool txResult = db->transaction({
//                                                    db->queryFormatting(queryTemplate1, auction_id),
//                                                    db->queryFormatting(queryTemplate2, current_price, seller_id),
//                                                    db->queryFormatting(queryTemplate3, current_bidder_id, item_id, item_quantity)
//                                            });
//            if (txResult) ret->emplace_back(std::pair<bool, std::pair<int, int>>(true, {seller_id, current_bidder_id}));
//        }
//    }
//    return std::move(ret);
//}