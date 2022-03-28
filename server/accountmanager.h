#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "account.h"

class AccountManager {
    private:
        std::unordered_map<int, Account*> activeAccounts;
        int accountID;
    
    public:
        AccountManager();

        int createAccount(std::string name, std::string password, Currency currency, float balance);
        void checkAccount(std::string name, std::string password, int accountID);
        void checkTargetAccount(std::string targetName, int targetAccountID);
        void closeAccount(std::string name, std::string password, int accountID);
        void changePassword(std::string name, std::string oldPassword, std::string newPassword, int accountID);
        float getExchangeRate(Currency currency, Currency targetCurrency);
        std::pair<Currency, float> checkAccountBalance(std::string name, std::string password, int accountID);
        std::pair<Currency, float> depositMoney(std::string name, std::string password, int accountID, Currency currency, float amount);
        std::pair<Currency, float> withdrawMoney(std::string name, std::string password, int accountID, Currency currency, float amount);
        std::pair<Currency, float> transferMoney(std::string name, std::string password, int accountID, std::string targetName, int targetAccountID, float amount);

        // to be removed in the future
        std::unordered_map<int, Account*> getActiveAccounts();
};