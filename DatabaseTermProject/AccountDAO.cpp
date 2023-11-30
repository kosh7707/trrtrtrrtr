#include "AccountDAO.h"

shared_ptr<Account> AccountDAO::getAccount(const string& id, const string& pw) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "select * from accounts where user_id='" + id + "' and user_pw='" + pw + "';";
    auto res = databaseConnection.selectQuery(query);

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
    string createdDate = PQgetvalue(res, 0, 3);
    string lastLogin = PQgetvalue(res, 0, 4);

    shared_ptr<Account> account = shared_ptr<Account>(new Account(accountId, userId, userPw, createdDate, lastLogin));
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
    string query = "insert into accounts(user_id, user_pw) values ('" + id + "', '" + pw + "')";
    bool res = databaseConnection.commandQuery(query);
    return res;
}
