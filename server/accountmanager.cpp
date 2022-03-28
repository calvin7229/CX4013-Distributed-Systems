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
void AccountManager::checkAccount(std::string name, std::string password, int accountID) /* throw(std::invalid_argument) */ {
    if (this->activeAccounts.find(accountID) != this->activeAccounts.end()) {
        Account* account = this->activeAccounts[accountID];
        
        if (account->getName() != name) {
            throw std::invalid_argument("Invalid account name corresponding to account number!");
        } else if (account->getPassword() != password) {
            throw std::invalid_argument("Invalid account password corresponding to account number!");
        } else {
            return;
        }
    } else {
        throw std::invalid_argument("Invalid account number!");
    }
}


// Function to check if account details entered matches a target account
void AccountManager::checkTargetAccount(std::string targetName, int targetAccountID) /* throw(std::invalid_argument) */ {
    if (this->activeAccounts.find(targetAccountID) != this->activeAccounts.end()) {
        Account* account = this->activeAccounts[targetAccountID];
        
        if (account->getName() != targetName) {
            throw std::invalid_argument("Invalid target account name corresponding to target account number!");
        } else {
            return;
        }
    } else {
        throw std::invalid_argument("Invalid target account number!");
    }
}


// Function to close (remove) an active account, if it exists
void AccountManager::closeAccount(std::string name, std::string password, int accountID) /* throw(std::invalid_argument) */ {
    try
    {
        AccountManager::checkAccount(name, password, accountID);
        this->activeAccounts.erase(accountID);
        return;
    }
    catch(const std::invalid_argument& ia)
    {
        throw std::invalid_argument(ia.what());
    }
}


// Function to change the password of an active account, if it exists
void AccountManager::changePassword(std::string name, std::string oldPassword, std::string newPassword, int accountID) /* throw(std::invalid_argument) */ {
    try
    {
        AccountManager::checkAccount(name, oldPassword, accountID);
        this->activeAccounts[accountID]->changePassword(newPassword);
        return;
    }
    catch(const std::invalid_argument& ia)
    {
        throw std::invalid_argument(ia.what());
    }
}


// Function to get the exchange rate from two currencies
float AccountManager::getExchangeRate(Currency currency, Currency targetCurrency) {
    if (currency == targetCurrency) {
        return 1;
    }
    
    std::unordered_map<std::pair<Currency, Currency>, float> rates = {
        {{SGD, MYR}, 3.09},
        {{SGD, GBP}, 0.56},
        {{SGD, USD}, 0.73},
        {{MYR, SGD}, 0.32},
        {{MYR, GBP}, 0.18},
        {{MYR, USD}, 0.24},
        {{GBP, SGD}, 1.78},
        {{GBP, MYR}, 5.51},
        {{GBP, USD}, 1.31},
        {{USD, SGD}, 1.36},
        {{USD, MYR}, 4.21},
        {{USD, GBP}, 0.76}
    };

    return rates[{currency, targetCurrency}];
}


// Function to get an account's balance, if it exists
std::pair<Currency, float> AccountManager::checkAccountBalance(std::string name, std::string password, int accountID) /* throw(std::invalid_argument) */ {
    try
    {
        AccountManager::checkAccount(name, password, accountID);
        Currency currency = this->activeAccounts[accountID]->getCurrency();
        float balance = this->activeAccounts[accountID]->getBalance();
        return std::pair<Currency, float>(currency, balance);
    }
    catch(const std::invalid_argument& ia)
    {
        throw std::invalid_argument(ia.what());
    }
}


// Function to deposit money and add to account's balance, if it exists
std::pair<Currency, float> AccountManager::depositMoney(std::string name, std::string password, int accountID, Currency currency, float amount) /* throw(std::invalid_argument) */ {
    try
    {
        AccountManager::checkAccount(name, password, accountID);
        Currency accountCurrency = this->activeAccounts[accountID]->getCurrency();
        float newAmount = amount * getExchangeRate(currency, accountCurrency);
        return this->activeAccounts[accountID]->depositMoney(newAmount);
    }
    catch(const std::invalid_argument& ia)
    {
        throw std::invalid_argument(ia.what());
    }
}


// Function to withdraw money and deduct from account's balance, if it exists
std::pair<Currency, float> AccountManager::withdrawMoney(std::string name, std::string password, int accountID, Currency currency, float amount) /* throw(std::invalid_argument) */ {
    try
    {
        AccountManager::checkAccount(name, password, accountID);
        Currency accountCurrency = this->activeAccounts[accountID]->getCurrency();
        float newAmount = amount * getExchangeRate(currency, accountCurrency);
        return this->activeAccounts[accountID]->withdrawMoney(newAmount);
    }
    catch(const std::invalid_argument& ia)
    {
        throw std::invalid_argument(ia.what());
    }
}


// Function to transfer money from one account to another, if it exists
std::pair<Currency, float> AccountManager::transferMoney(std::string name, std::string password, int accountID, std::string targetName, int targetAccountID, float amount) /* throw(std::invalid_argument) */ {
    try
    {
        AccountManager::checkAccount(name, password, accountID);
        AccountManager::checkTargetAccount(targetName, targetAccountID);

        auto account = this->activeAccounts[accountID]->withdrawMoney(amount);
        Currency currency = this->activeAccounts[accountID]->getCurrency();
        Currency targetCurrency = this->activeAccounts[targetAccountID]->getCurrency();
        float newAmount = amount * AccountManager::getExchangeRate(currency, targetCurrency);
        this->activeAccounts[targetAccountID]->depositMoney(newAmount);

        return account;
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