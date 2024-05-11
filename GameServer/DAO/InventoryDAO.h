#ifndef DATABASETERMPROJECT_INVENTORYDAO_H
#define DATABASETERMPROJECT_INVENTORYDAO_H

#include "../Model/Inventory.h"
#include "IDatabase.h"

class InventoryDAO {
    using itemsPtr = std::shared_ptr<std::unordered_map<int, std::pair<Item, int>>>;
public:
    InventoryDAO(std::shared_ptr<IDatabase> db) : db(db) {}
    std::unique_ptr<Inventory> readInventory(const int account_id);
    bool updateInventory(const int account_id, const std::unique_ptr<Inventory>& inventory);
    bool insertItem(const int account_id, const int item_id, const int quantity);
    bool deleteItem(const int account_id, const int item_id, const int quantity);
private:
    std::shared_ptr<IDatabase> db;
};


#endif //DATABASETERMPROJECT_INVENTORYDAO_H
