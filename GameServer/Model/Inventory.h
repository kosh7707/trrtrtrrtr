#ifndef GAMESERVER_INVENTORY_H
#define GAMESERVER_INVENTORY_H


#include <memory>
#include <iostream>
#include <unordered_map>
#include "Item.h"

class Inventory {
    using itemsPtr = std::shared_ptr<std::unordered_map<int, std::pair<Item, int>>>;
public:
    Inventory(itemsPtr items) : items(items) {}

    itemsPtr getItems() {
        return items;
    }

    void insertItem(const int item_id, const int quantity) {
        Item item(item_id);
        auto it = items->find(item_id);
        if (it == items->end()) items->emplace(item_id, std::make_pair(item, quantity));
        else it->second.second += quantity;
    }

    bool deleteItem(const int item_id, const int quantity) {
        auto it = items->find(item_id);
        if (it == items->end() or it->second.second < quantity) return false;
        it->second.second -= quantity;
        if (it->second.second == 0) items->erase(item_id);
        return true;
    }

private:
    itemsPtr    items;
};


#endif //GAMESERVER_INVENTORY_H