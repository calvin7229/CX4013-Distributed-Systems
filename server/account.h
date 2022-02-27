#pragma once 

#include <string>
#include <stdexcept>

enum Currency {
    SGD,
    MYR,
    GBP,
    USD
};

/*
I was thinking that the account number should be randomly generated on the server side on creation.
Server will keep track of a list of active accounts using their unique account number.
*/

class Account {
    private:
        std::string name, password;
        Currency currency;
        float balance;
        int accountNumber;

    public:
        Account(std::string name, std::string password, Currency currency, float balance, int accountNumber);

        std::string getName();
        std::string getPassword();
        std::string getCurrencyString();
        Currency getCurrency();
        static Currency getCurrency(int index);
        float getBalance();
        int getAccountNumber();     

        void changePassword(std::string newPassword);
        std::pair<Currency, float> depositMoney(float amount);
        std::pair<Currency, float> withdrawMoney(float amount);        
};