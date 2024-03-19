#ifndef DATABASETERMPROJECT_INVENTORYDAO_H
#define DATABASETERMPROJECT_INVENTORYDAO_H

#include <sstream>
#include "DatabaseConnection.h"
class InventoryDAO {
public:
    bool insertItem(const int account_id, const int item_id);
    bool deleteItem(const int account_id, const int item_id, const int quantity);
    std::string inventoryCheck(const int account_id);
    bool sellItem(const int account_id, const int item_id, const int quantity, const int buyNowPrice, const int startingBidPrice);
    std::pair<bool, std::pair<int, int>> buyNow(const int account_id, const int auction_id);
    std::pair<bool, int> bid(const int account_id, const int auction_id, const int price);
    bool breakItem(const int account_id, const int item_id, const int quantity);
};


#endif //DATABASETERMPROJECT_INVENTORYDAO_H
