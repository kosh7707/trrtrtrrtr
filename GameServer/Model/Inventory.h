#ifndef GAMESERVER_INVENTORY_H
#define GAMESERVER_INVENTORY_H


#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "Item.h"

class Inventory {
public:
    Inventory(int account_id, std::shared_ptr<std::unordered_map<int, Item>> items) : account_id(account_id), items(items) {}

    int getAccountId() {
        return account_id;
    }
    std::shared_ptr<std::unordered_map<int, Item>> getItems() {
        return items;
    }

    bool insertItem(const int item_id, const int quantity) {
        Item item(item_id, Item::idToScore[item_id], quantity);
        if (items->find(item_id) == items->end()) items->emplace(item_id, item);
        else (*items)[item_id].setQuantity((*items)[item_id].getQuantity() + item.getQuantity());
        return true;
    }

    bool insertItem(const Item& item) {
        int item_id = item.getItemId();
        if (items->find(item_id) == items->end()) items->emplace(item_id, item);
        else (*items)[item_id].setQuantity((*items)[item_id].getQuantity() + item.getQuantity());
        return true;
    }

    bool deleteItem(const int item_id, const int quantity) {
        Item item(item_id, Item::idToScore[item_id], quantity);
        if (items->find(item_id) != items->end() and (*items)[item_id].getQuantity() >= item.getQuantity()) {
            (*items)[item_id].setQuantity((*items)[item_id].getQuantity() - item.getQuantity());
            if ((*items)[item_id].getQuantity() == 0) items->erase(item_id);
            return true;
        }
        return false;
    }

    bool deleteItem(const Item& item) {
        int item_id = item.getItemId();
        if (items->find(item_id) != items->end() and (*items)[item_id].getQuantity() >= item.getQuantity()) {
            (*items)[item_id].setQuantity((*items)[item_id].getQuantity() - item.getQuantity());
            if ((*items)[item_id].getQuantity() == 0) items->erase(item_id);
            return true;
        }
        return false;
    }

    friend std::ostream& operator<< (std::ostream& os, const Inventory& inventory) {
        for (auto it : *inventory.items)
            os << std::to_string(it.first) << "," << std::to_string(it.second.getQuantity()) << "|";
        return os;
    }

private:
    int account_id;
    std::shared_ptr<std::unordered_map<int, Item>> items;
};


#endif //GAMESERVER_INVENTORY_H
