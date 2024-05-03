#ifndef GAMESERVER_INVENTORYSERVICE_H
#define GAMESERVER_INVENTORYSERVICE_H

#include "../DAO/AccountDAO.h"
#include "../DAO/InventoryDAO.h"

class InventoryService {
public:
    InventoryService(const std::shared_ptr<AccountDAO> accountDao, const std::shared_ptr<InventoryDAO> inventoryDao)
                        : accountDao(accountDao), inventoryDao(inventoryDao) {}

private:
    std::shared_ptr<AccountDAO> accountDao;
    std::shared_ptr<InventoryDAO> inventoryDao;
};


#endif //GAMESERVER_INVENTORYSERVICE_H
