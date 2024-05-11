#include "AuctionService.h"

using namespace AuctionSystem;

std::unique_ptr<std::vector<Auction>> AuctionService::readAuctions() {
    return std::move(auctionDao->readAuctions());
}

bool AuctionService::sellItem(std::unique_ptr<Inventory>& inventory, const int account_id, const int item_id, const int quantity, const int buyNowPrice, const int startingBidPrice) {
    if (auctionDao->addAuction(account_id, item_id, quantity, buyNowPrice, startingBidPrice)) {
        inventory->deleteItem(item_id, quantity);
        return true;
    }
    return false;
}

BidResult AuctionService::bid(std::unique_ptr<Account>& account, const int auction_id, const int price) {
    // TODO: caller가 outbid된 account의 balance를 update 해줘야 하는 구조. 고칠 필요가 있음.
    auto ret = auctionDao->bid(account->getAccountId(), auction_id, price);
    if (ret.success) account->setBalance(account->getBalance() - price);
    return ret;
}

BuyNowResult AuctionService::buyNow(std::unique_ptr<Account>& account, std::unique_ptr<Inventory>& inventory, const int auction_id) {
    // TODO: caller가 outbid된 account의 balance를 update 해줘야 하는 구조. 고칠 필요가 있음.
    auto ret = auctionDao->buyNow(account->getAccountId(), auction_id);
    if (ret.success) {
        account->setBalance(account->getBalance() - ret.price);
        inventory->insertItem(ret.item_id, ret.quantity);
    }
    return ret;
}
