#include "InventoryDAO.h"

bool InventoryDAO::insertItem(const string &id, const int item_id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string values = "( (select account_id from accounts where user_id = '" + id + "'), '" + to_string(item_id) + "', " + "'1')";
    string onConflict = "on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
    string query = "insert into inventory (account_id, item_id, quantity) values " + values + " " + onConflict;
    bool res = databaseConnection.commandQuery(query);
    return res;
}

string InventoryDAO::inventoryCheck(const string& id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "select * from inventory where account_id = (select account_id from accounts where user_id = '" + id + "')";
    auto res = databaseConnection.selectQuery(query);
    // TODO query ÆÄ½Ì ¹× return ±¸Çö
    return "";
}
