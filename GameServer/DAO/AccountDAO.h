#ifndef DATABASETERMPROJECT_ACCOUNTDAO_H
#define DATABASETERMPROJECT_ACCOUNTDAO_H


#include <memory>
#include "../Model/Account.h"

class AccountDAO {
public:
    AccountDAO(std::shared_ptr<IDatabase> db) : db(db) {}
    bool createAccount(const std::string& id, const std::string& pw);
    std::unique_ptr<Account> readAccount(const int account_id);
    std::unique_ptr<Account> readAccount(const std::string& id, const std::string& pw);
    bool updateAccount(const std::unique_ptr<Account>& account);
    bool updateAccountLastLogin(const int account_id);
    bool updateAccountBalance(const int account_id, const int balance);
    bool isExistAccount(const int account_id);
    bool isExistAccount(const std::string& id);
private:
    std::shared_ptr<IDatabase> db;
};


#endif //DATABASETERMPROJECT_ACCOUNTDAO_H
