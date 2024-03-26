#ifndef DATABASETERMPROJECT_ACCOUNTDAO_H
#define DATABASETERMPROJECT_ACCOUNTDAO_H


#include <memory>
#include "Account.h"
#include "DatabaseConnection.h"

class AccountDAO {
public:
    std::shared_ptr<Account> getAccount(const std::string& id, const std::string& pw);
    bool checkAccountExists(const std::string& id);
    bool registerAccount(const std::string& id, const std::string& pw);
    bool updateAccountLastLogin(const std::string& id);
};


#endif //DATABASETERMPROJECT_ACCOUNTDAO_H