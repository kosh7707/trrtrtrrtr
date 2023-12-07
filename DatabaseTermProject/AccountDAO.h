#ifndef DATABASETERMPROJECT_ACCOUNTDAO_H
#define DATABASETERMPROJECT_ACCOUNTDAO_H


#include <memory>
#include "Account.h"
#include "DatabaseConnection.h"

class AccountDAO {
public:
    shared_ptr<Account> getAccount(const string& id, const string& pw, const string& role);
    bool checkAccountExists(const string& id);
    bool registerAccount(const string& id, const string& pw, const string& role);
    bool updateAccountLastLogin(const string& id);
};


#endif //DATABASETERMPROJECT_ACCOUNTDAO_H
