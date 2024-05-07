#include "Client.h"

int Client::getAccountId() {
    return account->getAccountId();
}

void Client::setAccount(std::unique_ptr<Account> _account) {
    account = std::move(_account);
}

void Client::setIndex(const int _index) {
    index = _index;
}

std::string Client::getUserId() {
    return account->getUserId();
}

void Client::setInventory(std::unique_ptr<Inventory> _inventory) {
    inventory = std::move(_inventory);
}

std::unique_ptr<Inventory>& Client::getInventory() {
    return inventory;
}

std::unique_ptr<Account>& Client::getAccount() {
    return account;
}
