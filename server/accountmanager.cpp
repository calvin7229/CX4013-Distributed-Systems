#include "accountmanager.h"

// Class constructor
AccountManager::AccountManager() {
    this->activeAccounts.clear();
    this->accountID = 0;
}

// Function to create an account object
int AccountManager::createAccount(std::string name, std::string password, Currency currency, float balance) {
    Account* newAccount = new Account(name, password, currency, balance, this->accountID);
    this->activeAccounts[this->accountID] = newAccount;

    return this->accountID++;
}

// Function to check if account details entered matches an existing account
bool AccountManager::checkAccount(std::string name, std::string password, int accountNumber) /* throw(std::invalid_argument) */ {
    if (this->activeAccounts.find(accountNumber) != this->activeAccounts.end()) {
        Account* account = this->activeAccounts[accountNumber];
        
        if (account->getName() != name) {
            throw std::invalid_argument("Invalid account name corresponding to account number!");
        } else if (account->getPassword() != password) {
            throw std::invalid_argument("Invalid account password corresponding to account number!");
        } else {
            return true;
        }
    } else {
        throw std::invalid_argument("Invalid account number!");
    }
}

// Function to close(remove) an active account if it exists
bool AccountManager::closeAccount(std::string name, std::string password, int accountNumber) /* throw(std::invalid_argument) */ {
    try
    {
        if (AccountManager::checkAccount(name, password, accountNumber)) {
            this->activeAccounts.erase(accountNumber);
            return true;
        }
    }
    catch(const std::invalid_argument& ia)
    {
        throw std::invalid_argument(ia.what());
    }
}

// Function to change the password of an active account if it exists
bool AccountManager::changePassword(std::string name, std::string oldPassword, std::string newPassword, int accountNumber) /* throw(std::invalid_argument) */ {
    try
    {
        if (AccountManager::checkAccount(name, oldPassword, accountNumber)) {
            this->activeAccounts[accountNumber]->changePassword(newPassword);
            return true;
        }
    }
    catch(const std::invalid_argument& ia)
    {
        throw std::invalid_argument(ia.what());
    }
}

// Function to deposit money and add to account's balance if it exists
std::pair<Currency, float> AccountManager::depositMoney(std::string name, std::string password, int accountNumber, float amount) /* throw(std::invalid_argument) */ {
    try
    {
        if (AccountManager::checkAccount(name, password, accountNumber)) {
            return this->activeAccounts[accountNumber]->depositMoney(amount);
        }
    }
    catch(const std::invalid_argument& ia)
    {
        throw std::invalid_argument(ia.what());
    }
}

// Function to withdraw money and deduct from account's balance if it exists
std::pair<Currency, float> AccountManager::withdrawMoney(std::string name, std::string password, int accountNumber, float amount) /* throw(std::invalid_argument) */ {
    try
    {
        if (AccountManager::checkAccount(name, password, accountNumber)) {
            return this->activeAccounts[accountNumber]->withdrawMoney(amount);
        }
    }
    catch(const std::invalid_argument& ia)
    {
        throw std::invalid_argument(ia.what());
    }
}

// Getter: activeAccounts
std::unordered_map<int, Account*> AccountManager::getActiveAccounts() {
    return this->activeAccounts;
}