#include "InventoryDAO.h"

bool InventoryDAO::insertItem(const string &id, const int item_id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();

    const char* queryTemplate = "insert into inventory (account_id, item_id, quantity) values\n"
                                "( (select account_id from accounts where user_id = '%s'), '%d', '1')\n"
                                "on conflict (account_id, item_id) do update set quantity = inventory.quantity + excluded.quantity;";
    char queryBuffer[1024];
    snprintf(queryBuffer, sizeof(queryBuffer), queryTemplate, id.c_str(), item_id);
    string query(queryBuffer);

    bool res = databaseConnection.commandQuery(query);
    return res;
}

string InventoryDAO::inventoryCheck(const string& id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    std::ostringstream ss;

    string query1 = "select ac.user_id 아이디, ac.role 직업, ac.balance 잔고, ac.mana 마나 from accounts ac where user_id = '"+ id + "';";
    auto res1 = databaseConnection.selectQuery(query1);
    ss << "----------------------\n";
    for (const auto& row : res1) {
        for (const auto& p : row)
            ss << p.first << ":" << p.second << "\n";
        ss << "----------------------\n";
    }

    const char* queryTemplate = "select items.name 아이템이름, items.score 점수, items.mana 마나, items.gadget 가젯, buffs.name 버프이름, inventory.quantity 수량\n"
                                "from accounts ac, items, inventory, buffs\n"
                                "where ac.user_id = '%s'\n"
                                "  and ac.account_id = inventory.account_id\n"
                                "  and items.id = inventory.item_id\n"
                                "  and items.buff_id = buffs.id\n"
                                "order by items.id;";
    char queryBuffer[1024];
    snprintf(queryBuffer, sizeof(queryBuffer), queryTemplate, id.c_str());
    string query(queryBuffer);
    auto res2 = databaseConnection.selectQuery(query);
    ss << "----------------------\n";
    for (const auto& row : res2) {
        for (const auto& p : row)
            ss << p.first << ":" << p.second << "\n";
        ss << "----------------------\n";
    }
    return ss.str();
}
