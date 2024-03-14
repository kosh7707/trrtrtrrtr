#include "Account.h"

Account::Account(int accountId, const string &userId, const string &userPw, int balance,
                 const string &createdDate, const string &lastLogin) : accountId(accountId), userId(userId),
                                                                       userPw(userPw), balance(balance),
                                                                       createdDate(createdDate), lastLogin(lastLogin) {}

int Account::getAccountId() const {
    return accountId;
}

const string& Account::getUserId() const {
    return userId;
}

const string& Account::getUserPw() const {
    return userPw;
}

const string& Account::getCreatedDate() const {
    return createdDate;
}

const string& Account::getLastLogin() const {
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


