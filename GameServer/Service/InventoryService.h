#ifndef GAMESERVER_INVENTORYSERVICE_H
#define GAMESERVER_INVENTORYSERVICE_H

#include "../Constants/ItemConstant.h"
#include "../DAO/AccountDAO.h"
#include "../DAO/InventoryDAO.h"

class InventoryService {
public:
    InventoryService(const std::shared_ptr<AccountDAO> accountDao, const std::shared_ptr<InventoryDAO> inventoryDao)
                        : accountDao(accountDao), inventoryDao(inventoryDao) {}
    std::unique_ptr<Inventory> getInventory(const int account_id);
    bool grantItem(std::unique_ptr<Account>& account, std::unique_ptr<Inventory>& inventory, const int item_id, const int quantity);
    bool breakItem(std::unique_ptr<Account>& account, std::unique_ptr<Inventory>& inventory, const int item_id, const int quantity);
private:
    std::shared_ptr<AccountDAO> accountDao;
    std::shared_ptr<InventoryDAO> inventoryDao;
};


#endif //GAMESERVER_INVENTORYSERVICE_H
