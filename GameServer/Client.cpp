#include "Client.h"

void Client::setAccount(std::unique_ptr<Account> _account) {
    account = std::move(_account);
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

void Client::setIndex(int _index) {
    index = _index;
}

int Client::getIndex() {
    return index;
}
