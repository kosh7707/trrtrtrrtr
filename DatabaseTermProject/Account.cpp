#include "Account.h"

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

const string& Account::getRole() const {
    return role;
}

Account::Account(int accountId, const string& userId, const string& userPw, const string& role, int balance,
                 const string& createdDate, const string& lastLogin) : accountId(accountId), userId(userId),
                                                                       userPw(userPw), role(role), balance(balance),
                                                                       createdDate(createdDate), lastLogin(lastLogin) {}
