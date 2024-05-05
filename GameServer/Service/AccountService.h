#ifndef GAMESERVER_ACCOUNTSERVICE_H
#define GAMESERVER_ACCOUNTSERVICE_H

#include <string>
#include <memory>
#include "../Model/Account.h"
#include "../DAO/AccountDAO.h"

class AccountService {
public:
    AccountService(std::shared_ptr<AccountDAO> accountDao) : accountDao(accountDao) {}
    bool isExistAccount(const int account_id);
    bool isExistAccount(const std::string& id);
    std::unique_ptr<Account> loginAccount(const std::string& id, const std::string& pw);
    std::unique_ptr<Account> registerAccount(const std::string& id, const std::string& pw);
    bool updateAccountLastLogin(const int account_id);
    bool updateAccountLastLogin(const std::unique_ptr<Account>& account);
private:
    std::shared_ptr<AccountDAO> accountDao;
};


#endif //GAMESERVER_ACCOUNTSERVICE_H
