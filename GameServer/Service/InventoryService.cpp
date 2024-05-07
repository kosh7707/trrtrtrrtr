#include "InventoryService.h"


bool InventoryService::grantItem(std::unique_ptr<Account>& account, std::unique_ptr<Inventory> inventory, const int item_id, const int quantity) {
    return inventory->insertItem(item_id, quantity) and inventoryDao->insertItem(account->getAccountId(), item_id, quantity);
}

bool InventoryService::breakItem(std::unique_ptr<Account>& account, std::unique_ptr<Inventory> inventory, const int item_id, const int quantity) {
    bool res1 = inventory->deleteItem(item_id, quantity);
    int account_id = account->getAccountId();
    bool res2 = inventoryDao->deleteItem(account_id, item_id, quantity);
    int score = Item::idToScore[item_id];
    account->setScore(account->getScore() + score);
    bool res3 = accountDao->updateAccount(account);
    return res1 and res2 and res3;
}

std::unique_ptr<Inventory> InventoryService::getInventory(const int account_id) {
    return std::move(inventoryDao->readInventory(account_id));
}
