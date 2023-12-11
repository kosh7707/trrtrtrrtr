#include "PermissionStoreDAO.h"

string PermissionStoreDAO::openPermissionStore() {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    ostringstream ss;

    auto res = databaseConnection.selectQuery("select permission_id, description 설명, price 가격 from permission_store;");
    ss << "Permission Store -----\n";
    for (const auto& row : res) {
        for (const auto& p : row)
            ss << p.first << ":" << p.second << "\n";
        ss << "----------------------\n";
    }
    return ss.str();
}

bool PermissionStoreDAO::buyPermission(const string& id, int permission_id) {
    // TODO 구현구현구현 그리고 구현 필요
    return false;
}
