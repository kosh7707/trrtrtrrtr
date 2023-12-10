#ifndef DATABASETERMPROJECT_INVENTORYDAO_H
#define DATABASETERMPROJECT_INVENTORYDAO_H

#include <sstream>
#include "DatabaseConnection.h"
class InventoryDAO {
public:
    bool insertItem(const string& id, const int item_id);
    bool deleteItem(const string& id, const int item_id, const int quantity);
    string inventoryCheck(const string& id);
    bool sellItem(const string& id, const int item_id, const int quantity, const int buyNowPrice, const int startingBidPrice);
    bool buyNow(const string& id, const int auction_id);
    bool breakItem(const string& id, const int item_id, const int quantity);
    bool bid(const string& id, const string& auction_id, const string& price);
};


#endif //DATABASETERMPROJECT_INVENTORYDAO_H
