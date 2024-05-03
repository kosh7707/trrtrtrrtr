#ifndef GAMESERVER_INVENTORY_H
#define GAMESERVER_INVENTORY_H


#include <vector>
#include <memory>

class Inventory {
public:
    Inventory(int account_id, std::shared_ptr<std::vector<std::pair<int, int>>> items) : account_id(account_id), items(items) {}

    int getAccountId() {
        return account_id;
    }
    std::shared_ptr<std::vector<std::pair<int, int>>> getItems() {
        return items;
    }

    void insertItem(std::pair<int, int> item) {
        items->emplace_back(item);
    }

private:
    int account_id;
    std::shared_ptr<std::vector<std::pair<int, int>>> items;
};


#endif //GAMESERVER_INVENTORY_H
