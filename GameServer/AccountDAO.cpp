#include "AccountDAO.h"

std::shared_ptr<Account> AccountDAO::getAccount(const std::string& id, const std::string& pw) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    std::string query = "select * from accounts where user_id='" + id + "' and user_pw='" + pw + "';";
    auto res = databaseConnection.selectQuery(query);

    if (res.size() != 1) return nullptr;
    int accountId = stoi(res[0]["account_id"]);
    std::string userId = res[0]["user_id"];
    std::string userPw = res[0]["user_pw"];
    int balance = stoi(res[0]["balance"]);
    std::string createdDate = res[0]["create_date"];
    std::string lastLogin = res[0]["last_login"];

    std::shared_ptr<Account> account = std::shared_ptr<Account>(new Account(accountId, userId, userPw, balance, createdDate, lastLogin));
    return account;
}

bool AccountDAO::checkAccountExists(const std::string& id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    std::string query = "select * from accounts where user_id='" + id + "';";
    auto res = databaseConnection.selectQuery(query);
    return res.size() == 1;
}

bool AccountDAO::registerAccount(const std::string& id, const std::string& pw) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    bool res = databaseConnection.commandQuery("insert into accounts(user_id, user_pw) values ('" + id + "', '" + pw + "');");
    return res;
}

bool AccountDAO::updateAccountLastLogin(const std::string& id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    std::string query = "update accounts set last_login=CURRENT_TIMESTAMP where user_id='" + id + "';";
    bool res = databaseConnection.commandQuery(query);
    return res;
}
