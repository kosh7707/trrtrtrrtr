#ifndef DATABASETERMPROJECT_INVENTORYDAO_H
#define DATABASETERMPROJECT_INVENTORYDAO_H

#include "DatabaseConnection.h"
class InventoryDAO {
public:
    bool insertItem(const string& id, const int item_id);
    string inventoryCheck(const string& id);
};


#endif //DATABASETERMPROJECT_INVENTORYDAO_H
