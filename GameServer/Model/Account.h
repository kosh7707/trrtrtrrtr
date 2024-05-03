#ifndef DATABASETERMPROJECT_ACCOUNT_H
#define DATABASETERMPROJECT_ACCOUNT_H

#include <string>

class Account {
public:
    Account() {}
    Account(int accountId, const std::string& userId, const std::string& userPw, int balance,
            const std::string& createdDate, const std::string& lastLogin);
    int                     getAccountId() const;
    const std::string&      getUserId() const;
    const std::string&      getUserPw() const;
    const std::string&      getCreatedDate() const;
    const std::string&      getLastLogin() const;
    int                     getBalance() const;
    void                    setBalance(int b);
private:
    int             accountId;
    std::string     userId;
    std::string     userPw;
    int             balance;
    std::string     createdDate;
    std::string     lastLogin;
};


#endif //DATABASETERMPROJECT_ACCOUNT_H
