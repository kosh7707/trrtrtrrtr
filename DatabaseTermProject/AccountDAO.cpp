#include "AccountDAO.h"

shared_ptr<Account> AccountDAO::getAccount(const string& id, const string& pw, const string& role) {
    string insert_role;
    if (role == "0") insert_role = "merchant";
    else if (role == "1") insert_role = "mage";
    else insert_role = "hacker";

    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "select * from accounts where user_id='" + id + "' and user_pw='" + pw + "' and role='"+ insert_role + "';";
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
    string _role = PQgetvalue(res, 0, 3);
    int balance = stoi(string(PQgetvalue(res, 0, 4)));
    string createdDate = PQgetvalue(res, 0, 5);
    string lastLogin = PQgetvalue(res, 0, 6);

    shared_ptr<Account> account = shared_ptr<Account>(new Account(accountId, userId, userPw, _role, balance, createdDate, lastLogin));
    return account;
}

bool AccountDAO::checkAccountExists(const string& id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "select * from accounts where user_id='" + id + "';";
    auto res = databaseConnection.selectQuery(query);
    return PQntuples(res) == 1;
}

bool AccountDAO::registerAccount(const string& id, const string& pw, const string& role) {
    string insert_role;
    if (role == "0") insert_role = "merchant";
    else if (role == "1") insert_role = "mage";
    else insert_role = "hacker";

    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    const PGconn* conn = databaseConnection.getConn();
    try {
        // begin transaction
        if (PQtransactionStatus(conn) != PQTRANS_IDLE)
            throw runtime_error("begin transaction error, status not IDLE");
        databaseConnection.commandQuery("BEGIN;");

        // insert DB Table user account
        string query1 = "insert into accounts(user_id, user_pw, role) values ('" + id + "', '" + pw + "', '" + insert_role + "');";
        bool res1 = databaseConnection.commandQuery(query1);
        if (!res1) throw runtime_error("exec error, query: " + query1);

        // create DBMS user account
        string query2 = "create user " + id + " with password '" + pw + "';";
        bool res2 = databaseConnection.commandQuery(query2);
        if (!res2) throw runtime_error("exec error, query: " + query2);

        // grant user role
        string query3 = "grant " + insert_role + " to " + id + ";";
        bool res3 = databaseConnection.commandQuery(query3);
        if (!res3) throw runtime_error("exec error, query: " + query3);

        // transaction success, commit
        databaseConnection.commandQuery("COMMIT;");
        return true;
    } catch (const exception& err) {
        // transaction failed, rollback
        cout << err.what() << endl;
        if (PQtransactionStatus(conn) == PQTRANS_INTRANS) {
            cout << "transaction rollback" << endl;
            databaseConnection.commandQuery("ROLLBACK;");
        }
        return false;
    }
}

bool AccountDAO::updateAccountLastLogin(const string& id) {
    IDatabaseConnection& databaseConnection = DatabaseConnection::getInstance();
    string query = "update accounts set last_login=CURRENT_TIMESTAMP where user_id='" + id + "';";
    bool res = databaseConnection.commandQuery(query);
    return res;
}
