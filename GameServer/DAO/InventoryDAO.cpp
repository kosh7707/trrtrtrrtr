#include "InventoryDAO.h"

std::unique_ptr<Inventory> InventoryDAO::readInventory(int account_id) {
    const char* queryTemplate = "select * from inventory_view where account_id = %d;";
    std::string query = db->queryFormatting(queryTemplate, account_id);
    auto res = db->selectQuery(query);
    itemsPtr items = std::make_unique<std::unordered_map<int, std::pair<Item, int>>>();
    for (auto row : *res) {
        int item_id = stoi(row["item_id"]);
        Item item(item_id);
        int quantity = stoi(row["quantity"]);
        items->emplace(item_id, std::make_pair(item, quantity));
    }
    std::unique_ptr<Inventory> inventory = std::make_unique<Inventory>(items);
    return std::move(inventory);
}

bool InventoryDAO::updateInventory(const int account_id, const std::unique_ptr<Inventory>& inventory) {
    auto items = inventory->getItems();
    bool ret = true;
    for (const auto& item : *items) {
        const char* queryTemplate = "insert into inventory values (%d, %d, %d) on conflict do update (account_id, item_id) set quantity = %d";
        std::string query = db->queryFormatting(queryTemplate, account_id, item.first, item.second.second);
        ret = ret and db->commandQuery(query);
    }
    return ret;
}

bool InventoryDAO::insertItem(const int account_id, const int item_id, const int quantity) {
    const char* queryTemplate = "insert into inventory (account_id, item_id, quantity) values ('%d', '%d', '%d')\n"
                                "on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
    std::string query = db->queryFormatting(queryTemplate, account_id, item_id, quantity);
    return db->commandQuery(query);
}

bool InventoryDAO::deleteItem(const int account_id, const int item_id, const int quantity) {
    const char* queryTemplate1 = "select * from inventory i where i.account_id = %d and item_id = %d and quantity >= %d;";
    std::string query1 = db->queryFormatting(queryTemplate1, account_id, item_id, quantity);
    auto res1 = db->selectQuery(query1);

    if (res1->size() != 1) return false;
    std::string query2;
    if (quantity == stoi((*res1)[0]["quantity"])) {
        const char* queryTemplate2 = "delete from inventory i where i.account_id = %d and i.item_id = %d;";
        query2 = db->queryFormatting(queryTemplate2, account_id, item_id);
    }
    else {
        const char* queryTemplate2 = "update inventory set quantity = quantity - %d where account_id = %d and item_id = %d;";
        query2 = db->queryFormatting(queryTemplate2, quantity, account_id, item_id);
    }
    return db->commandQuery(query2);
}


