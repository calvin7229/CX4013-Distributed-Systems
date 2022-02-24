#include "account.h"

// Class constructor
Account::Account(std::string _name, std::string _password, Currency _currency, float _balance, int _accountNumber) {
    this->name = _name;
    this->password = _password;
    this->currency = _currency;
    this->balance = _balance;
    this->accountNumber = _accountNumber;
}

// Getter: name
std::string Account::getName() {
    return this->name;
}

// Getter: password
std::string Account::getPassword() {
    return this->password;
}

// Getter: balance
int Account::getBalance() {
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
void Account::depositMoney(float amount) {
    this->balance += amount;
}

// Function to withdraw money and deduct from account's balance
void Account::withdrawMoney(float amount) {
    if (amount > this->balance) {
        throw std::invalid_argument("Attempting to withdraw more money than currently available...");
    } else {
        this->balance -= amount;
    }
}