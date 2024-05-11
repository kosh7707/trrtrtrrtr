#include "Account.h"

Account::Account(int accountId, const std::string &userId, const std::string &userPw, int balance,
                 int score, const std::string& createdDate, const std::string &lastLogin) : accountId(accountId), userId(userId),
                                                                       userPw(userPw), balance(balance), score(score),
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

int Account::getBalance() const {
    return balance;
}


void Account::setBalance(int b) {
    balance = b;
}

void Account::addBalance(int b) {
    balance += b;
}

void Account::subBalance(int b) {
    balance -= b;
}

int Account::getScore() const {
    return score;
}

void Account::setScore(int s) {
    score = s;
}

void Account::addScore(int s) {
    score += s;
}

void Account::subScore(int s) {
    score -= s;
}



