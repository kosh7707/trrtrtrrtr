#include "AccountDAO.h"

shared_ptr<Account> AccountDAO::getAccount(const string& id, const string& pw) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "select * from accounts where user_id='" + id + "' and user_pw='" + pw + "';";
    auto res = databaseConnection.selectQuery(query);

    if (res.size() != 1) return nullptr;
    int accountId = stoi(res[0]["account_id"]);
    string userId = res[0]["user_id"];
    string userPw = res[0]["user_pw"];
    int balance = stoi(res[0]["balance"]);
    string createdDate = res[0]["create_date"];
    string lastLogin = res[0]["last_login"];

    shared_ptr<Account> account = shared_ptr<Account>(new Account(accountId, userId, userPw, balance, createdDate, lastLogin));
    return account;
}

bool AccountDAO::checkAccountExists(const string& id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "select * from accounts where user_id='" + id + "';";
    auto res = databaseConnection.selectQuery(query);
    return res.size() == 1;
}

bool AccountDAO::registerAccount(const string& id, const string& pw) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    bool res = databaseConnection.commandQuery("insert into accounts(user_id, user_pw) values ('" + id + "', '" + pw + "');");
    return res;
}

bool AccountDAO::updateAccountLastLogin(const string& id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "update accounts set last_login=CURRENT_TIMESTAMP where user_id='" + id + "';";
    bool res = databaseConnection.commandQuery(query);
    return res;
}
