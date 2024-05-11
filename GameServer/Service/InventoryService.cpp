#include "InventoryService.h"


bool InventoryService::grantItem(std::unique_ptr<Account>& account, std::unique_ptr<Inventory>& inventory, const int item_id, const int quantity) {
    if (inventoryDao->insertItem(account->getAccountId(), item_id, quantity)) {
        inventory->insertItem(item_id, quantity);
        return true;
    }
    return false;
}

bool InventoryService::breakItem(std::unique_ptr<Account>& account, std::unique_ptr<Inventory>& inventory, const int item_id, const int quantity) {
    int account_id = account->getAccountId();
    if (inventoryDao->deleteItem(account_id, item_id, quantity)) {
        inventory->deleteItem(item_id, quantity);
        account->setScore(account->getScore() + ItemConstant::getItemScore(item_id) * quantity);
        accountDao->updateAccount(account);
        return true;
    }
    return false;
}

std::unique_ptr<Inventory> InventoryService::getInventory(const int account_id) {
    return std::move(inventoryDao->readInventory(account_id));
}
