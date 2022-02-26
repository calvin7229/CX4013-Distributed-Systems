#include <iostream>
#include "accountmanager.h"

std::pair<std::string, std::string> getNameAndPassword() {
    std::string name, password;

    std::cin.ignore();
    std::cout << "\nEnter The Account's Name: ";
    std::getline(std::cin, name);

    std::cin.clear();
    std::cin.sync();
    std::cout << "Enter The Account's Password: ";
    std::getline(std::cin, password);

    return std::pair<std::string, std::string>{name, password};
}

std::string getCurrency(Currency currency) {
    switch (currency) {
        case SGD:
            return "SGD";
            break;
        case MYR:
            return "MYR";
            break;
        case GBP:
            return "GBP";
            break;
        case USD:
            return "USD";
            break;
    }

    return "SGD";
}

int main() {
    AccountManager AM = AccountManager();
    bool core = true;
    int userInput;
    std::string name, password;

    std::cout << "--- CLI test driver for Server ---" << std::endl;
    do
    {
        std::cout << "\n- Main Menu -" << std::endl;
        std::cout << "0. EXIT" << std::endl;
        std::cout << "1. Create New Account" << std::endl;
        std::cout << "2. Change Password" << std::endl;
        std::cout << "3. Deposit Money" << std::endl;
        std::cout << "4. Withdraw Money" << std::endl;
        std::cout << "5. Close Existing Account" << std::endl;
        std::cout << "666. List All Existing Account (ILLEGAL)\n" << std::endl;
        std::cout << "Enter Your Choice: ";
        std::cin >> userInput;

        switch (userInput) {
            case 0:
                core = false;
                break;

            case 1: {
                Currency currency = SGD;
                int currencyInt;
                float balance;

                std::pair<std::string, std::string> p = getNameAndPassword();
                name = p.first;
                password = p.second;
                std::cout << "Enter The Account's Currency {1. SGD, 2. MYR, 3. GBP, 4. USD}: ";
                std::cin >> currencyInt;
                switch (currencyInt) {
                case 1:
                    currency = SGD;
                    break;
                case 2:
                    currency = MYR;
                    break;
                case 3:
                    currency = GBP;
                    break;
                case 4:
                    currency = USD;
                    break;    
                default:
                    break;
                }
                std::cout << "Enter The Account's Initial Balance: ";
                std::cin >> balance;

                int accountNumber = AM.createAccount(name, password, currency, balance);
                std::cout << "\nAccount Created! Account Number is " << accountNumber << "." << std::endl;
                break;
            }

            case 2: {
                std::string newPassword;
                int accountNumber;
                
                std::pair<std::string, std::string> p = getNameAndPassword();
                name = p.first;
                password = p.second;
                std::cout << "Enter The Account's New Password: ";
                std::cin >> newPassword;
                std::cout << "Enter The Account's Number (ID): ";
                std::cin >> accountNumber;

                try
                {
                    bool success = AM.changePassword(name, password, newPassword, accountNumber);
                    std::cout << "\nPassword Changed Successfully!" << std::endl;
                }
                catch(const std::invalid_argument& ia)
                {
                    std::cerr << "\n" << ia.what() << std::endl;
                }
                break;
            }

            case 3: {
                int accountNumber;
                float amount;
                
                std::pair<std::string, std::string> p = getNameAndPassword();
                name = p.first;
                password = p.second;
                std::cout << "Enter The Account's Number (ID): ";
                std::cin >> accountNumber;
                std::cout << "Enter The Amount to Deposit: ";
                std::cin >> amount;

                try
                {
                    std::pair<Currency, float> cur = AM.depositMoney(name, password, accountNumber, amount);
                    std::cout << "\nAmount Deposited Successfully!" << std::endl;
                    std::cout << "Account Balance: " << getCurrency(cur.first) << ". " << cur.second << std::endl;
                }
                catch(const std::invalid_argument& ia)
                {
                    std::cerr << "\n" << ia.what() << std::endl;
                }
                break;
            }

            case 4: {
                int accountNumber;
                float amount;
                
                std::pair<std::string, std::string> p = getNameAndPassword();
                name = p.first;
                password = p.second;
                std::cout << "Enter The Account's Number (ID): ";
                std::cin >> accountNumber;
                std::cout << "Enter The Amount to Withdraw: ";
                std::cin >> amount;

                try
                {
                    std::pair<Currency, float> cur = AM.withdrawMoney(name, password, accountNumber, amount);
                    std::cout << "\nAmount Withdrawn Successfully!" << std::endl;
                    std::cout << "Account Balance: " << getCurrency(cur.first) << ". " << cur.second << std::endl;
                }
                catch(const std::invalid_argument& ia)
                {
                    std::cerr << "\n" << ia.what() << std::endl;
                }
                break;
            }

            case 5: {
                int accountNumber;
                
                std::pair<std::string, std::string> p = getNameAndPassword();
                name = p.first;
                password = p.second;
                std::cout << "Enter The Account's Number (ID): ";
                std::cin >> accountNumber;

                try
                {
                    bool success = AM.closeAccount(name, password, accountNumber);
                    std::cout << "\nAccount Closed Successfully!" << std::endl;
                }
                catch(const std::invalid_argument& ia)
                {
                    std::cerr << "\n" << ia.what() << std::endl;
                }
                break;
            }    
            
            case 666: {
                std::unordered_map<int, Account*> aa = AM.getActiveAccounts();
                std::unordered_map<int, Account*>::iterator it;

                if (aa.begin() == aa.end()) {
                    std::cout << "\nNo Active Accounts!" << std::endl;
                } else {
                    int index = 0;
                    for (it = aa.begin(); it != aa.end(); it++) {
                        int key = it->first;
                        Account* value = it->second;
                        std::cout << "\n" << index++ << ". ";
                        std::cout << "Account Number: " << key << "\t";
                        std::cout << "Name: " << value->getName() << "\t";
                        std::cout << "Password: " << value->getPassword() << "\t";
                        std::cout << "Balance: " << getCurrency(value->getCurrency()) << ". " << value->getBalance() << std::endl; 
                    }
                }
                break;
            }
                
            default:
                std::cout << "\nPlease Enter Something Valid, PLEASE!" << std::endl;
        }
    } while (core);

    std::cout << "\nBYE!" << std::endl;

    return 0;
}