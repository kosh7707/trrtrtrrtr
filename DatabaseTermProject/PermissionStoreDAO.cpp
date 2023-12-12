#include "PermissionStoreDAO.h"

string PermissionStoreDAO::openPermissionStore() {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    ostringstream ss;

    auto res = databaseConnection.selectQuery("select permission_id, description 설명, price 가격(마나) from permission_store;");
    ss << "----------------------\n";
    for (const auto& row : res) {
        for (const auto& p : row)
            ss << p.first << ":" << p.second << "\n";
        ss << "----------------------\n";
    }
    return ss.str();
}

bool PermissionStoreDAO::buyPermission(const string& id, int permission_id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();

    auto res = databaseConnection.selectQuery("select * from permission_store where permission_id = " + to_string(permission_id))[0];

    vector<string> queries;

    const char* queryTemplate1 = res["query"].c_str();
    string query1 = DatabaseConnection::queryFormatting(queryTemplate1, id.c_str());
    queries.emplace_back(query1);

    int price = stoi(res["price"]);
    const char* queryTemplate2 = "update accounts set mana = mana - %d where user_id = '%s'";
    string query2 = DatabaseConnection::queryFormatting(queryTemplate2, price, id.c_str());
    queries.emplace_back(query2);

    return databaseConnection.transaction(queries);
}
