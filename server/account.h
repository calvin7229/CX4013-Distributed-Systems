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
        Account(std::string _name, std::string _password, Currency _currency, float _balance, int _accountNumber);

        std::string getName();
        std::string getPassword();
        int getBalance();
        int getAccountNumber();     

        void changePassword(std::string newPassword);
        void depositMoney(float amount);
        void withdrawMoney(float amount);        
};