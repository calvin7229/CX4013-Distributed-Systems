#pragma once 

#include <string>
#include <stdexcept>
#include <iostream>

enum Currency {
    SGD,
    MYR,
    GBP,
    USD
};

class Account {
    private:
        std::string name, password;
        Currency currency;
        float balance;
        int accountID;

    public:
        Account(std::string name, std::string password, Currency currency, float balance, int accountID);

        std::string getName();
        std::string getPassword();
        Currency getCurrency();
        static Currency getCurrency(int index);
        static int getCurrencyIndex(Currency currency);
        static std::string getCurrencyString(Currency currency);

        float getBalance();
        int getAccountNumber();     

        void changePassword(std::string newPassword);
        std::pair<Currency, float> depositMoney(float amount);
        std::pair<Currency, float> withdrawMoney(float amount);      
};