#ifndef DATABASETERMPROJECT_INVENTORYDAO_H
#define DATABASETERMPROJECT_INVENTORYDAO_H

#include <sstream>
#include "DatabaseConnection.h"
class InventoryDAO {
public:
    // TODO const string& id -> const int account_id 로 모든 함수의 인자를 바꾸고 수정해줘야함.
    bool insertItem(const string& id, const int item_id);
    bool deleteItem(const string& id, const int item_id, const int quantity);
    string inventoryCheck(const string& id);
    bool sellItem(const string& id, const int item_id, const int quantity, const int buyNowPrice, const int startingBidPrice);
    pair<bool, pair<int, int>> buyNow(const string& id, const int auction_id);
    pair<bool, int> bid(const int account_id, const int auction_id, const int price);
    bool breakItem(const string& id, const int item_id, const int quantity);
};


#endif //DATABASETERMPROJECT_INVENTORYDAO_H
