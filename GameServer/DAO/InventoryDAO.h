#ifndef DATABASETERMPROJECT_INVENTORYDAO_H
#define DATABASETERMPROJECT_INVENTORYDAO_H

#include "../Model/Inventory.h"
#include "IDatabase.h"

class InventoryDAO {
public:
    InventoryDAO(std::shared_ptr<IDatabase> db) : db(db) {}
    std::unique_ptr<Inventory> readInventory(const int account_id);
    bool updateInventory(const std::unique_ptr<Inventory>& inventory);
    bool insertItem(const int account_id, const int item_id, const int quantity);
    bool deleteItem(const int account_id, const int item_id, const int quantity);
private:
    std::shared_ptr<IDatabase> db;
//    bool sellItem(const int account_id, const int item_id, const int quantity, const int buyNowPrice, const int startingBidPrice);
//    std::pair<bool, std::pair<int, int>> buyNow(const int account_id, const int auction_id);
//    std::pair<bool, int> bid(const int account_id, const int auction_id, const int price);
//    bool breakItem(const int account_id, const int item_id, const int quantity);
};


#endif //DATABASETERMPROJECT_INVENTORYDAO_H
