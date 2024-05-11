#ifndef DATABASETERMPROJECT_ACCOUNT_H
#define DATABASETERMPROJECT_ACCOUNT_H

#include <string>

class Account {
public:
    Account() {}
    Account(int accountId, const std::string& userId, const std::string& userPw, int balance,
            int score, const std::string& createdDate, const std::string& lastLogin);
    int                     getAccountId() const;
    const std::string&      getUserId() const;
    const std::string&      getUserPw() const;
    const std::string&      getCreatedDate() const;
    const std::string&      getLastLogin() const;
    int                     getBalance() const;
    void                    setBalance(int b);
    void                    addBalance(int b);
    void                    subBalance(int b);
    int                     getScore() const;
    void                    setScore(int s);
    void                    addScore(int s);
    void                    subScore(int s);
private:
    int             accountId;
    std::string     userId;
    std::string     userPw;
    int             balance;
    int             score;
    std::string     createdDate;
    std::string     lastLogin;
};


#endif //DATABASETERMPROJECT_ACCOUNT_H
