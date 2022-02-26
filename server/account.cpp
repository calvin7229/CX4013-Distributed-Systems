#include "account.h"

// Class constructor
Account::Account(std::string name, std::string password, Currency currency, float balance, int accountNumber) {
    this->name = name;
    this->password = password;
    this->currency = currency;
    this->balance = balance;
    this->accountNumber = accountNumber;
}

// Getter: name
std::string Account::getName() {
    return this->name;
}

// Getter: password
std::string Account::getPassword() {
    return this->password;
}

// Getter: currency
Currency Account::getCurrency() {
    return this->currency;
}

// Getter: balance
float Account::getBalance() {
    return this->balance;
}

// Getter: account number
int Account::getAccountNumber() {
    return this->accountNumber;
}

// Function to change the account's password
void Account::changePassword(std::string newPassword) {
    this->password = newPassword;
}

// Function to deposit money and add to account's balance
std::pair<Currency, float> Account::depositMoney(float amount) {
    this->balance += amount;

    return std::pair<Currency, float>(this->currency, this->balance);
}

// Function to withdraw money and deduct from account's balance
std::pair<Currency, float> Account::withdrawMoney(float amount) /* throw(std::invalid_argument) */ {
    if (amount > this->balance) {
        throw std::invalid_argument("Attempting to withdraw more money than currently available...");
    } else {
        this->balance -= amount;
    }

    return std::pair<Currency, float>(this->currency, this->balance);
}