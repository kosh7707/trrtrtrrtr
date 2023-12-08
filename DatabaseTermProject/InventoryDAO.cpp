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

    // 유저 정보 쿼리
    string query1 = "select ac.role, ac.balance, ac.mana from accounts ac where user_id = '"+ id + "';";
    auto res1 = databaseConnection.selectQuery(query1);
    for (int i=0; i<PQntuples(res1); i++) {
        for (int j=0; j<PQnfields(res1); j++)
            ss << PQgetvalue(res1, i, j) << ",";
        ss << "\n";
    }

    // 유저 인벤토리 쿼리
    const char* queryTemplate = "select items.name, items.score, items.mana, items.gadget, buffs.name, inventory.quantity\n"
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
    for (int i=0; i<PQntuples(res2); i++) {
        for (int j=0; j<PQnfields(res2); j++)
            ss << PQgetvalue(res2, i, j) << ",";
        ss << "\n";
    }
    return ss.str();;
}
