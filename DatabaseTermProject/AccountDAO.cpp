#include "AccountDAO.h"

shared_ptr<Account> AccountDAO::getAccount(const string& id, const string& pw, const string& _role) {
    string update_role;
    if (_role == "0") update_role = "merchant";
    else if (_role == "1") update_role = "mage";
    else update_role = "hacker";

    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query1 = "update accounts set role='" + update_role + "' where user_id='" + id + "';";
    auto update_res = databaseConnection.commandQuery(query1);
    string query2 = "select * from accounts where user_id='" + id + "' and user_pw='" + pw + "';";
    auto res = databaseConnection.selectQuery(query2);

    if (PQntuples(res) == 0) {
        cout << "There is no account id: " << id << ", pw: " << pw << endl;
        return nullptr;
    }
    else if (PQntuples(res) >= 2) {
        cout << "There is more than two accounts" << endl;
        return nullptr;
    }
    int accountId = stoi(PQgetvalue(res, 0, 0));
    string userId = PQgetvalue(res, 0, 1);
    string userPw = PQgetvalue(res, 0, 2);
    string role = PQgetvalue(res, 0, 3);
    int balance = stoi(string(PQgetvalue(res, 0, 4)));
    string createdDate = PQgetvalue(res, 0, 5);
    string lastLogin = PQgetvalue(res, 0, 6);

    shared_ptr<Account> account = shared_ptr<Account>(new Account(accountId, userId, userPw, role, balance, createdDate, lastLogin));
    return account;
}

bool AccountDAO::checkAccountExists(const string& id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "select * from accounts where user_id='" + id + "';";
    auto res = databaseConnection.selectQuery(query);
    return PQntuples(res) == 1;
}

bool AccountDAO::registerAccount(const string& id, const string& pw) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "insert into accounts(user_id, user_pw) values ('" + id + "', '" + pw + "');";
    bool res = databaseConnection.commandQuery(query);
    return res;
}

bool AccountDAO::updateAccountLastLogin(const string& id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "update accounts set last_login=CURRENT_TIMESTAMP where user_id='" + id + "';";
    bool res = databaseConnection.commandQuery(query);
    return res;
}
