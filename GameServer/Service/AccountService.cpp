#include "AccountService.h"

bool AccountService::isExistAccount(const int account_id) {
    return accountDao->isExistAccount(account_id);
}

bool AccountService::isExistAccount(const std::string& id) {
    return accountDao->isExistAccount(id);
}

std::unique_ptr<Account> AccountService::loginAccount(const std::string& id, const std::string& pw) {
    return std::move(accountDao->readAccount(id, pw));
}

std::unique_ptr<Account> AccountService::registerAccount(const std::string& id, const std::string& pw) {
    if (accountDao->createAccount(id, pw))
        return std::move(accountDao->readAccount(id, pw));
    return nullptr;
}

bool AccountService::updateAccountLastLogin(const int account_id) {
    return accountDao->updateAccountLastLogin(account_id);
}

bool AccountService::updateAccountLastLogin(const std::unique_ptr<Account>& account) {
    return accountDao->updateAccountLastLogin(account->getAccountId());
}


