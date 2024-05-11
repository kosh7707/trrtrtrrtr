#ifndef GAMESERVER_AUCTIONDAO_H
#define GAMESERVER_AUCTIONDAO_H


#include <memory>
#include "IDatabase.h"

namespace AuctionSystem {
    struct Auction {
        int auction_id;
        int item_id;
        int item_quantity;
        int seller_id;
        std::string start_time;
        std::string end_time;
        int current_price;
        int current_bidder_id;
        int buy_now_price;
    };

    struct BidResult {
        bool success;
        int prev_bidder_id;
        int prev_price;
    };

    struct BuyNowResult {
        bool success;
        int seller_id;
        int item_id;
        int quantity;
        int price;
        int prev_bidder_id;
        int prev_price;
    };

    class AuctionDAO {
    public:
        AuctionDAO(std::shared_ptr<IDatabase> db) : db(db) {}
        std::unique_ptr<std::vector<Auction>> readAuctions();
        bool            addAuction(const int account_id, const int item_id, const int quantity, const int buyNowPrice, const int startingBidPrice);
        BidResult       bid(const int account_id, const int auction_id, const int price);
        BuyNowResult    buyNow(const int account_id, const int auction_id);
        // std::unique_ptr<std::vector<std::pair<bool, Auction>>> outdatedItemCheck();
    private:
        std::shared_ptr<IDatabase> db;
    };
}


#endif //GAMESERVER_AUCTIONDAO_H
