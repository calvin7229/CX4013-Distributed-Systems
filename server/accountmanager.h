#pragma once

#include <string>
#include <unordered_map>
#include "account.h"

class AccountManager {
    private:
        std::unordered_map<int, Account*> activeAccounts;
        int accountID;
    
    public:
        AccountManager();

        int createAccount(std::string name, std::string password, Currency currency, float balance);
        bool checkAccount(std::string name, std::string password, int accountNumber);
        bool closeAccount(std::string name, std::string password, int accountNumber);
        bool changePassword(std::string name, std::string oldPassword, std::string newPassword, int accountNumber);
        std::pair<Currency, float> depositMoney(std::string name, std::string password, int accountNumber, float amount);
        std::pair<Currency, float> withdrawMoney(std::string name, std::string password, int accountNumber, float amount);

        // to be removed in the future
        std::unordered_map<int, Account*> getActiveAccounts();
};