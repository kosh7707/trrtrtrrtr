#include "AccountDAO.h"

bool AccountDAO::createAccount(const std::string& id, const std::string& pw) {
    return db->commandQuery("insert into accounts(user_id, user_pw) values ('" + id + "', '" + pw + "');");
}

std::unique_ptr<Account> AccountDAO::readAccount(const int account_id) {
    auto res = db->selectQuery("select * from accounts where account_id = " + std::to_string(account_id) + ";");
    if (res.size() != 1) return nullptr;
    auto account = res[0];
    auto user_id = account["user_id"];
    auto user_pw = account["user_pw"];
    auto balance = stoi(account["balance"]);
    auto score = stoi(account["socre"]);
    auto created_date = account["created_date"];
    auto last_login = account["last_login"];
    std::unique_ptr<Account> pAccount = std::make_unique<Account>(account_id, user_id, user_pw, balance, score, created_date, last_login);
    return std::move(pAccount);
}

std::unique_ptr<Account> AccountDAO::readAccount(const std::string& id, const std::string& pw) {
    auto res = db->selectQuery("select * from accounts where user_id = '" + id + "' and user_pw = '" + pw + "';");
    if (res.size() != 1) return nullptr;
    auto account = res[0];
    auto account_id = stoi(account["account_id"]);
    auto user_id = account["user_id"];
    auto user_pw = account["user_pw"];
    auto balance = stoi(account["balance"]);
    auto score = stoi(account["score"]);
    auto created_date = account["created_date"];
    auto last_login = account["last_login"];
    std::unique_ptr<Account> pAccount = std::make_unique<Account>(account_id, user_id, user_pw, balance, score, created_date, last_login);
    return std::move(pAccount);
}

bool AccountDAO::updateAccount(const std::unique_ptr<Account>& account) {
    auto account_id = account->getAccountId();
    auto user_id = account->getUserId();
    auto user_pw = account->getUserPw();
    auto balance = account->getBalance();
    auto score = account->getScore();
    auto created_date = account->getCreatedDate();
    auto last_login = account->getLastLogin();
    const char* queryTemplate = "update accounts set user_id = '%s', user_pw = '%s', balance = %d, score = %d, created_date = '%s', last_login = '%s' where account_id = %d";
    std::string query = db->queryFormatting(queryTemplate, user_id.c_str(), user_pw.c_str(), balance, score, created_date.c_str(), last_login.c_str(), account_id);
    return db->commandQuery(query);
}

bool AccountDAO::updateAccountLastLogin(const int account_id) {
    return db->commandQuery("update accounts set last_login=CURRENT_TIMESTAMP where account_id = " + std::to_string(account_id) + ";");
}

bool AccountDAO::updateAccountBalance(const int account_id, const int balance) {
    return db->commandQuery("update accounts set balance=" + std::to_string(balance) + " where account_id = " + std::to_string(account_id) + ";");
}

bool AccountDAO::isExistAccount(const int account_id) {
    auto res = db->selectQuery("select * from accounts where account_id = " + std::to_string(account_id) + ";");
    return res.size() == 1;
}

bool AccountDAO::isExistAccount(const std::string& id) {
    auto res = db->selectQuery("select * from accounts where user_id = '" + id + "';");
    return res.size() == 1;
}

