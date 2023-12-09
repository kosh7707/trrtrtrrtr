#include "AccountDAO.h"

shared_ptr<Account> AccountDAO::getAccount(const string& id, const string& pw, const string& role) {
    string insert_role;
    if (role == "0") insert_role = "merchant";
    else if (role == "1") insert_role = "mage";
    else insert_role = "hacker";

    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "select * from accounts where user_id='" + id + "' and user_pw='" + pw + "' and role='"+ insert_role + "';";
    auto res = databaseConnection.selectQuery(query);

    if (res.size() != 1) return nullptr;
    int accountId = stoi(res[0]["account_id"]);
    string userId = res[0]["user_id"];
    string userPw = res[0]["user_pw"];
    string _role = res[0]["role"];
    int balance = stoi(res[0]["balance"]);
    int mana = stoi(res[0]["mana"]);
    string createdDate = res[0]["create_date"];
    string lastLogin = res[0]["last_login"];

    shared_ptr<Account> account = shared_ptr<Account>(new Account(accountId, userId, userPw, _role, balance, mana, createdDate, lastLogin));
    return account;
}

bool AccountDAO::checkAccountExists(const string& id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "select * from accounts where user_id='" + id + "';";
    auto res = databaseConnection.selectQuery(query);
    return res.size() == 1;
}

bool AccountDAO::registerAccount(const string& id, const string& pw, const string& role) {
    string insert_role;
    if (role == "0") insert_role = "merchant";
    else if (role == "1") insert_role = "mage";
    else insert_role = "hacker";

    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    vector<string> queries;
    queries.emplace_back("insert into accounts(user_id, user_pw, role) values ('" + id + "', '" + pw + "', '" + insert_role + "');");
    queries.emplace_back("create user " + id + " with password '" + pw + "';");
    queries.emplace_back("grant " + insert_role + " to " + id + ";");
    bool res = databaseConnection.transaction(queries);
    return res;
}

bool AccountDAO::updateAccountLastLogin(const string& id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "update accounts set last_login=CURRENT_TIMESTAMP where user_id='" + id + "';";
    bool res = databaseConnection.commandQuery(query);
    return res;
}
