#ifndef GAMESERVER_AUCTIONSERVICE_H
#define GAMESERVER_AUCTIONSERVICE_H


#include "../DAO/AuctionDAO.h"
#include "../Model/Inventory.h"
#include "../Model/Account.h"

namespace AuctionSystem {
    class AuctionService {
    public:
        AuctionService(std::shared_ptr<AuctionDAO> auctionDao) : auctionDao(auctionDao) {}
        std::unique_ptr<std::vector<Auction>> readAuctions();
        bool sellItem(std::unique_ptr<Inventory>& inventory, const int account_id, const int item_id, const int quantity, const int buyNowPrice, const int startingBidPrice);
        BidResult bid(std::unique_ptr<Account>& account, const int auction_id, const int price);
        BuyNowResult buyNow(std::unique_ptr<Account>& account, std::unique_ptr<Inventory>& inventory, const int auction_id);
        // std::unique_ptr<std::vector<std::pair<bool, std::pair<int, int>>>> outdatedItemCheck();
    private:
        std::shared_ptr<AuctionDAO> auctionDao;
    };
};



#endif //GAMESERVER_AUCTIONSERVICE_H
