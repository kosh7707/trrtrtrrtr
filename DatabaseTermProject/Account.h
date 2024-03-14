#ifndef DATABASETERMPROJECT_ACCOUNT_H
#define DATABASETERMPROJECT_ACCOUNT_H

#include <string>

using namespace std;

class Account {
public:
    Account();
    Account(int accountId, const string& userId, const string& userPw, int balance,
            const string& createdDate, const string& lastLogin);

    int             getAccountId() const;
    const string&   getUserId() const;
    const string&   getUserPw() const;
    const string&   getCreatedDate() const;
    const string&   getLastLogin() const;
    int             getBalance();
    void            setBalance(int b);
private:
    int     accountId;
    string  userId;
    string  userPw;
    int     balance;
    string  createdDate;
    string  lastLogin;
};


#endif //DATABASETERMPROJECT_ACCOUNT_H
