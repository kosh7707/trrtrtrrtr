#include "Account.h"

Account::Account(int accountId, const std::string &userId, const std::string &userPw, int balance,
                 const std::string &createdDate, const std::string &lastLogin) : accountId(accountId), userId(userId),
                                                                       userPw(userPw), balance(balance),
                                                                       createdDate(createdDate), lastLogin(lastLogin) {}

int Account::getAccountId() const {
    return accountId;
}

const std::string& Account::getUserId() const {
    return userId;
}

const std::string& Account::getUserPw() const {
    return userPw;
}

const std::string& Account::getCreatedDate() const {
    return createdDate;
}

const std::string& Account::getLastLogin() const {
    return lastLogin;
}
Account::Account() {
    userId = "svr";
}

int Account::getBalance() {
    return balance;
}

void Account::setBalance(int b) {
    balance = b;
}


