#include "handler.h"

using namespace constants;

const int BASIC_RESPONSE_SIZE = ID_SIZE + ACK_SIZE;


// Class constructor
Handler::Handler(int mode, double failureRate) {
    std::random_device device;

    this->AM = AccountManager();
    this->monitor = Monitor();
    this->mt = std::mt19937(device());
    this->distribution = std::uniform_real_distribution<double> (0.0, 1.0);

    this->cache.clear();
    this->responseID = 0;
    this->mode = mode;
    this->failureRate = failureRate;
}


// For debugging use
void printB(char c) {
    for (int i = 7; i >= 0; --i) {
        std::cout << ((c & (1 << i))? '1' : '0');
    }
}


// Getter: responseID
int Handler::getResponseID() {
    return this->responseID;
}


// Function to inquire about client's request
void Handler::inquire(UDPServer& server) {
    // Inquire message header information
    int receiveStatus, bodySize;
    char header[HEADER_SIZE];

    try {
        receiveStatus = server.receive(header, HEADER_SIZE, NO_TIMEOUT);
        std::cout << "Header Received!" << std::endl;
    } catch (std::string err) {
        std::cerr << err << std::endl;
        return;
    }

    // Inquire message body information
    bodySize = transform::unmarshalInt(header);
    char body[bodySize];

    try {
        receiveStatus = server.receive(body, bodySize, NO_TIMEOUT);
        std::cout << "Body Received!" << std::endl;
    } catch (std::string err) {
        std::cerr << err << std::endl;
        return;
    }

    // Inquire service type
    char* buffer = body;
    int requestID = transform::unmarshalInt(buffer);
    std::cout << "Request ID: " << requestID << std::endl;
    buffer += INT_SIZE;
    int serviceType = transform::unmarshalInt(buffer);
    std::cout << "Service Type: " << serviceType << std::endl;
    buffer += INT_SIZE;

    switch (serviceType) {
        case 1:
            serviceOpen(server, buffer, requestID);
            break;
        case 2:
            serviceClose(server, buffer, requestID);
            break;
        case 3:
            serviceDeposit(server, buffer, requestID);
            break;
        case 4:
            serviceWithdraw(server, buffer, requestID);
            break;
        case 5:
            serviceTransfer(server, buffer, requestID);
            break;
        case 6:
            serviceBalance(server, buffer, requestID);
            break;
        case 7:
            serviceSubscribe(server, buffer, requestID);
        default:
            break;
    }

    std::cout << std::endl;
}   


// Function to answer client's request with a response
void Handler::answer(UDPServer& server, char* header, char* body, int bodySize) {
    double rand = this->distribution(mt);
    std::cout << "Generated Failute Rate = " << rand * 100 << "%" << std::endl;
    
    if(rand > this->failureRate) {
        // for (int i = 0; i < 4; i++) {
        //     std::cout << "bin at index " << i << ", val: " << std::endl;
        //     printB(*(body+i));
        //     std::cout << std::endl;
        // }

        // Send header
        server.send(header, HEADER_SIZE);

        // Send body
        server.send(body, bodySize);

        // Increase response ID's value
        this->responseID++;
    } else {
        std::cout << "--- Failure Occured! ---" << std::endl;
    }
}


// Function to notify a subscriber
void Handler::notify(UDPServer& server, char* header, char* body, int bodySize, sockaddr_in clientAddr) {
    // Send header
    server.send(header, HEADER_SIZE, clientAddr);

    // Send body
    server.send(body, bodySize, clientAddr);
}


// Function to handle duplicate requests
void Handler::accessDuplicates(UDPServer& server, unsigned long clientAddr, int requestID) {
    struct in_addr ca;
    ca.s_addr = clientAddr;
    char *dot_ip = inet_ntoa(ca);

    std::cout << "Duplicate found!" << std::endl; 
    std::cout << "Client Address: " << UDPServer::getAddressString(ca) << std::endl; 
    std::cout << "Request ID: " << requestID << std::endl; 
    std::string s = cache[{clientAddr, requestID}];
    int bodySize = s.size();

    // Prepare response message
    char header[HEADER_SIZE];
    char body[bodySize];
    
    transform::marshalInt(bodySize, header);

    for (int i = 0; i < bodySize; i++) {
        body[i] = s[i];
    }

    answer(server, header, body, bodySize);
}


// Function to handle service: Open a new bank account
/*
    Response structure:
        Header
            * Description *     * Type *        * Size (bytes) *
            Size of body        int             4
        Body
            * Description *     * Type *        * Size (bytes) *
            Response ID         int             4
            Acknowledgement     char            1
            Account ID          int             4
*/
void Handler::serviceOpen(UDPServer& server, char* info, int requestID) {
    auto clientAddr = server.getClientAddr().sin_addr.s_addr;
    auto clientA = server.getClientAddr().sin_addr;

    // Check if request ID already in cache
    if (mode == 1 && (cache.find({clientAddr, requestID}) != cache.end())) {
        accessDuplicates(server, clientAddr, requestID);
    } else {
        // Extract essential information to open an account
        std::string name, password;
        Currency currency;
        float balance;
        int n, temp;

        n = transform::unmarshalInt(info);              info += INT_SIZE;
        name = transform::unmarshalString(info, n);     info += n;
        std::cout << "Account Name: " << name << std::endl;

        n = transform::unmarshalInt(info);              info += INT_SIZE;
        password = transform::unmarshalString(info, n); info += n;
        std::cout << "Account Password: " << password << std::endl;

        temp = transform::unmarshalInt(info);           info += INT_SIZE;
        currency = Account::getCurrency(temp);
        std::cout << "Account Currency: " << Account::getCurrencyString(currency) << std::endl;

        balance = transform::unmarshalFloat(info);      info += INT_SIZE;
        std::cout << "Account Balance: " << balance << std::endl;

        // Create the account through Account Manager
        int accountID = AM.createAccount(name, password, currency, balance);
        std::cout << "Account ID: " << accountID << std::endl;

        // Prepare callback message
        std::string callback;
        std::stringstream ss;
        ss << "Account ID " << accountID << " OPENED by ";
        ss << UDPServer::getAddressString(clientA);
        callback = ss.str();

        updateSubscribers(server, callback);

        // Prepare response message
        int bodySize = BASIC_RESPONSE_SIZE + INT_SIZE;
        char header[HEADER_SIZE];
        char body[bodySize];

        std::cout << "Request ID not found!" << std::endl;
        transform::marshalInt(bodySize, header);
        char* buffer = body;

        transform::marshalInt(getResponseID(), buffer); buffer += ID_SIZE;
        transform::marshalChar(ACK_SUCCESS, buffer);    buffer += ACK_SIZE;
        transform::marshalInt(accountID, buffer);       buffer += INT_SIZE;

        // Save buffer result into cache
        if (mode == 1) {
            std::string s = std::string(body, bodySize);

            cache[{clientAddr, requestID}] = s;
        }

        answer(server, header, body, bodySize);
    }
}


// Function to handle service: Close an existing bank account
/*
    Response structure (Success):
        Header
            * Description *     * Type *        * Size (bytes) *
            Size of body        int             4
        Body
            * Description *     * Type *        * Size (bytes) *
            Response ID         int             4
            Acknowledgement     char            1
    
    Response structure (Failure):
        Header
            * Description *     * Type *        * Size (bytes) *
            Size of body        int             4
        Body
            * Description *     * Type *        * Size (bytes) *
            Response ID         int             4
            Acknowledgement     char            1
            Error message size  int             4
            Error message       string          Error message size
*/
void Handler::serviceClose(UDPServer& server, char* info, int requestID) {
    auto clientAddr = server.getClientAddr().sin_addr.s_addr;
    auto clientA = server.getClientAddr().sin_addr;

    // Check if request ID already in cache
    if (mode == 1 && (cache.find({clientAddr, requestID}) != cache.end())) {
        accessDuplicates(server, clientAddr, requestID);
    } else {
        // Extract essential information to close an account
        std::string name, password;
        int accountID, n;

        n = transform::unmarshalInt(info);              info += INT_SIZE;
        name = transform::unmarshalString(info, n);     info += n;
        std::cout << "Account Name: " << name << std::endl;

        n = transform::unmarshalInt(info);              info += INT_SIZE;
        password = transform::unmarshalString(info, n); info += n;
        std::cout << "Account Password: " << password << std::endl;

        accountID = transform::unmarshalInt(info);      info += INT_SIZE;
        std::cout << "Account ID: " << accountID << std::endl;

        // Close the account through Account Manager
        int bodySize = BASIC_RESPONSE_SIZE;
        bool closeStatus;
        std::string err;

        try
        {
            AM.closeAccount(name, password, accountID);
            closeStatus = true;
        }
        catch(const std::invalid_argument& ia)
        {
            closeStatus = false;
            err = std::string(ia.what());
            bodySize += INT_SIZE + err.size();
            std::cout << err << std::endl;
        }

        // Prepare response message
        char header[HEADER_SIZE];
        char body[bodySize];

        transform::marshalInt(bodySize, header);
        char* buffer = body;

        transform::marshalInt(getResponseID(), buffer);     
        buffer += ID_SIZE;

        if (closeStatus) {
            // Prepare callback message
            std::string callback;
            std::stringstream ss;
            ss << "Account ID " << accountID << " CLOSED by ";
            ss << UDPServer::getAddressString(clientA);
            callback = ss.str();

            updateSubscribers(server, callback);

            transform::marshalChar(ACK_SUCCESS, buffer);    buffer += ACK_SIZE;
        } else {
            transform::marshalChar(ACK_FAIL, buffer);       buffer += ACK_SIZE;
            transform::marshalInt(err.size(), buffer);      buffer += INT_SIZE;
            transform::marshalString(err, buffer);          buffer += err.size();
        }

        // Save buffer result into cache
        if (mode == 1) {
            std::string s = std::string(body, bodySize);

            cache[{clientAddr, requestID}] = s;
        }

        answer(server, header, body, bodySize);
    }
}


// Function to handle service: Deposit money into an existing bank account
/*
    Response structure (Success):
        Header
            * Description *     * Type *        * Size (bytes) *
            Size of body        int             4
        Body
            * Description *     * Type *        * Size (bytes) *
            Response ID         int             4
            Acknowledgement     char            1
            Currency            int             4
            Balance             float           4
    
    Response structure (Failure):
        Header
            * Description *     * Type *        * Size (bytes) *
            Size of body        int             4
        Body
            * Description *     * Type *        * Size (bytes) *
            Response ID         int             4
            Acknowledgement     char            1
            Error message size  int             4
            Error message       string          Error message size
*/
void Handler::serviceDeposit(UDPServer& server, char* info, int requestID) {
    auto clientAddr = server.getClientAddr().sin_addr.s_addr;
    auto clientA = server.getClientAddr().sin_addr;
    
    // Check if request ID already in cache
    if (mode == 1 && (cache.find({clientAddr, requestID}) != cache.end())) {
        accessDuplicates(server, clientAddr, requestID);
    } else {
        // Extract essential information to deposit money into an account
        std::string name, password;
        Currency currency;
        int accountID, n, temp;
        float amount;

        n = transform::unmarshalInt(info);              info += INT_SIZE;
        name = transform::unmarshalString(info, n);     info += n;
        std::cout << "Account Name: " << name << std::endl;

        n = transform::unmarshalInt(info);              info += INT_SIZE;
        password = transform::unmarshalString(info, n); info += n;
        std::cout << "Account Password: " << password << std::endl;

        accountID = transform::unmarshalInt(info);      info += INT_SIZE;
        std::cout << "Account ID: " << accountID << std::endl;

        temp = transform::unmarshalInt(info);           info += INT_SIZE;
        currency = Account::getCurrency(temp);
        std::cout << "Currency: " << Account::getCurrencyString(currency) << std::endl;

        amount = transform::unmarshalFloat(info);       info += INT_SIZE;
        std::cout << "Deposit Amount: " << amount << std::endl;

        // Deposit the amount through Account Manager
        int bodySize = BASIC_RESPONSE_SIZE;
        int currencyInt;
        float balance;
        bool depositStatus;
        std::pair<Currency, float> accountDetails;
        std::string err;

        try
        {
            accountDetails = AM.depositMoney(name, password, accountID, currency, amount);
            bodySize += INT_SIZE + FLOAT_SIZE;
            depositStatus = true;
        }
        catch(const std::invalid_argument& ia)
        {
            depositStatus = false;
            err = std::string(ia.what());
            bodySize += INT_SIZE + err.size();
            std::cout << err << std::endl;
        }

        // Prepare response message
        char header[HEADER_SIZE];
        char body[bodySize];

        transform::marshalInt(bodySize, header);
        char* buffer = body;

        transform::marshalInt(getResponseID(), buffer);     
        buffer += ID_SIZE;

        if (depositStatus) {
            // Prepare callback message
            std::string callback;
            std::stringstream ss;
            ss << Account::getCurrencyString(currency) << " " << amount;
            ss << " DEPOSITED into Account ID " << accountID << " by ";
            ss << UDPServer::getAddressString(clientA);
            callback = ss.str();

            updateSubscribers(server, callback);

            transform::marshalChar(ACK_SUCCESS, buffer);    buffer += ACK_SIZE;
            currencyInt = Account::getCurrencyIndex(accountDetails.first);
            transform::marshalInt(currencyInt, buffer);     buffer += INT_SIZE;
            balance = accountDetails.second;
            transform::marshalFloat(balance, buffer);       buffer += FLOAT_SIZE;  
        } else {
            transform::marshalChar(ACK_FAIL, buffer);       buffer += ACK_SIZE;
            transform::marshalInt(err.size(), buffer);      buffer += INT_SIZE;
            transform::marshalString(err, buffer);          buffer += err.size();
        }

        // Save buffer result into cache
        if (mode == 1) {
            std::string s = std::string(body, bodySize);

            cache[{clientAddr, requestID}] = s;
        }

        answer(server, header, body, bodySize);
    }
}


// Function to handle service: Withdraw money from an existing bank account
/*
    Response structure (Success):
        Header
            * Description *     * Type *        * Size (bytes) *
            Size of body        int             4
        Body
            * Description *     * Type *        * Size (bytes) *
            Response ID         int             4
            Acknowledgement     char            1
            Currency            int             4
            Balance             float           4
    
    Response structure (Failure):
        Header
            * Description *     * Type *        * Size (bytes) *
            Size of body        int             4
        Body
            * Description *     * Type *        * Size (bytes) *
            Response ID         int             4
            Acknowledgement     char            1
            Error message size  int             4
            Error message       string          Error message size
*/
void Handler::serviceWithdraw(UDPServer& server, char* info, int requestID) {
    auto clientAddr = server.getClientAddr().sin_addr.s_addr;
    auto clientA = server.getClientAddr().sin_addr;

    // Check if request ID already in cache
    if (mode == 1 && (cache.find({clientAddr, requestID}) != cache.end())) {
        accessDuplicates(server, clientAddr, requestID);
    } else {
        // Extract essential information to withdraw money from an account
        std::string name, password;
        Currency currency;
        int accountID, n, temp;
        float amount;

        n = transform::unmarshalInt(info);              info += INT_SIZE;
        name = transform::unmarshalString(info, n);     info += n;
        std::cout << "Account Name: " << name << std::endl;

        n = transform::unmarshalInt(info);              info += INT_SIZE;
        password = transform::unmarshalString(info, n); info += n;
        std::cout << "Account Password: " << password << std::endl;

        accountID = transform::unmarshalInt(info);      info += INT_SIZE;
        std::cout << "Account ID: " << accountID << std::endl;

        temp = transform::unmarshalInt(info);           info += INT_SIZE;
        currency = Account::getCurrency(temp);
        std::cout << "Currency: " << Account::getCurrencyString(currency) << std::endl;

        amount = transform::unmarshalFloat(info);       info += FLOAT_SIZE;
        std::cout << "Withdraw Amount: " << amount << std::endl;

        // Withdraw the amount through Account Manager
        int bodySize = BASIC_RESPONSE_SIZE;
        int currencyInt;
        float balance;
        bool withdrawStatus;
        std::pair<Currency, float> accountDetails;
        std::string err;

        try
        {
            accountDetails = AM.withdrawMoney(name, password, accountID, currency, amount);
            bodySize += INT_SIZE + FLOAT_SIZE;
            withdrawStatus = true;
        }
        catch(const std::invalid_argument& ia)
        {
            withdrawStatus = false;
            err = std::string(ia.what());
            bodySize += INT_SIZE + err.size();
            std::cout << err << std::endl;
        }

        // Prepare response message
        char header[HEADER_SIZE];
        char body[bodySize];

        transform::marshalInt(bodySize, header);
        char* buffer = body;

        transform::marshalInt(getResponseID(), buffer);     
        buffer += ID_SIZE;

        if (withdrawStatus) {
            // Prepare callback message
            std::string callback;
            std::stringstream ss;
            ss << Account::getCurrencyString(currency) << " " << amount;
            ss << " WITHDRAWN from Account ID " << accountID << " by ";
            ss << UDPServer::getAddressString(clientA);
            callback = ss.str();

            updateSubscribers(server, callback);

            transform::marshalChar(ACK_SUCCESS, buffer);    buffer += ACK_SIZE;
            currencyInt = Account::getCurrencyIndex(accountDetails.first);
            transform::marshalInt(currencyInt, buffer);     buffer += INT_SIZE;
            balance = accountDetails.second;
            transform::marshalFloat(balance, buffer);       buffer += FLOAT_SIZE;  
        } else {
            transform::marshalChar(ACK_FAIL, buffer);       buffer += ACK_SIZE;
            transform::marshalInt(err.size(), buffer);      buffer += INT_SIZE;
            transform::marshalString(err, buffer);          buffer += err.size();
        }

        // Save buffer result into cache
        if (mode == 1) {
            std::string s = std::string(body, bodySize);

            cache[{clientAddr, requestID}] = s;
        }

        answer(server, header, body, bodySize);
    }
}


// Function to handle service: Transfer money from one account to another 
/*
    Response structure (Success):
        Header
            * Description *     * Type *        * Size (bytes) *
            Size of body        int             4
        Body
            * Description *     * Type *        * Size (bytes) *
            Response ID         int             4
            Acknowledgement     char            1
            Currency            int             4
            Balance             float           4
    
    Response structure (Failure):
        Header
            * Description *     * Type *        * Size (bytes) *
            Size of body        int             4
        Body
            * Description *     * Type *        * Size (bytes) *
            Response ID         int             4
            Acknowledgement     char            1
            Error message size  int             4
            Error message       string          Error message size
*/
void Handler::serviceTransfer(UDPServer& server, char* info, int requestID) {
    auto clientAddr = server.getClientAddr().sin_addr.s_addr;
    auto clientA = server.getClientAddr().sin_addr;

    // Check if request ID already in cache
    if (mode == 1 && (cache.find({clientAddr, requestID}) != cache.end())) {
        accessDuplicates(server, clientAddr, requestID);
    } else {
        // Extract essential information to transfer money from one account to another
        std::string name, targetName, password;
        int accountID, targetAccountID, n;
        float amount;

        n = transform::unmarshalInt(info);                  info += INT_SIZE;
        name = transform::unmarshalString(info, n);         info += n;
        std::cout << "Account Name: " << name << std::endl;

        n = transform::unmarshalInt(info);                  info += INT_SIZE;
        password = transform::unmarshalString(info, n);     info += n;
        std::cout << "Account Password: " << password << std::endl;

        accountID = transform::unmarshalInt(info);          info += INT_SIZE;
        std::cout << "Account ID: " << accountID << std::endl;

        n = transform::unmarshalInt(info);                  info += INT_SIZE;
        targetName = transform::unmarshalString(info, n);   info += n;
        std::cout << "Target Account Name: " << targetName << std::endl;

        targetAccountID = transform::unmarshalInt(info);    info += INT_SIZE;
        std::cout << "Target Account ID: " << targetAccountID << std::endl;

        amount = transform::unmarshalFloat(info);           info += FLOAT_SIZE;
        std::cout << "Transfer Amount: " << amount << std::endl;

        // Transfer the amount through Account Manager
        int bodySize = BASIC_RESPONSE_SIZE;
        int currency;
        float balance;
        bool transferStatus;
        std::pair<Currency, float> accountDetails;
        std::string err;

        try
        {
            accountDetails = AM.transferMoney(name, password, accountID, targetName, targetAccountID, amount);
            bodySize += INT_SIZE + FLOAT_SIZE;
            transferStatus = true;
        }
        catch(const std::invalid_argument& ia)
        {
            transferStatus = false;
            err = std::string(ia.what());
            bodySize += INT_SIZE + err.size();
            std::cout << err << std::endl;
        }

        // Prepare response message
        char header[HEADER_SIZE];
        char body[bodySize];

        transform::marshalInt(bodySize, header);
        char* buffer = body;

        transform::marshalInt(getResponseID(), buffer);     
        buffer += ID_SIZE;

        if (transferStatus) {
            // Prepare callback message
            std::string callback;
            std::stringstream ss;
            ss << Account::getCurrencyString(accountDetails.first) << " " << amount;
            ss << " TRANSFERED from Account ID " << accountID << " to " << targetAccountID << " by ";
            ss << UDPServer::getAddressString(clientA);
            callback = ss.str();

            updateSubscribers(server, callback);

            transform::marshalChar(ACK_SUCCESS, buffer);    buffer += ACK_SIZE;
            currency = Account::getCurrencyIndex(accountDetails.first);
            transform::marshalInt(currency, buffer);        buffer += INT_SIZE;
            balance = accountDetails.second;
            transform::marshalFloat(balance, buffer);       buffer += FLOAT_SIZE;  
        } else {
            transform::marshalChar(ACK_FAIL, buffer);       buffer += ACK_SIZE;
            transform::marshalInt(err.size(), buffer);      buffer += INT_SIZE;
            transform::marshalString(err, buffer);          buffer += err.size();
        }

        // Save buffer result into cache
        if (mode == 1) {
            std::string s = std::string(body, bodySize);

            cache[{clientAddr, requestID}] = s;
        }

        answer(server, header, body, bodySize);
    }
}


// Function to handle service: Check an account's balance
/*
    Response structure (Success):
        Header
            * Description *     * Type *        * Size (bytes) *
            Size of body        int             4
        Body
            * Description *     * Type *        * Size (bytes) *
            Response ID         int             4
            Acknowledgement     char            1
            Currency            int             4
            Balance             float           4
    
    Response structure (Failure):
        Header
            * Description *     * Type *        * Size (bytes) *
            Size of body        int             4
        Body
            * Description *     * Type *        * Size (bytes) *
            Response ID         int             4
            Acknowledgement     char            1
            Error message size  int             4
            Error message       string          Error message size
*/
void Handler::serviceBalance(UDPServer& server, char* info, int requestID) {
    auto clientAddr = server.getClientAddr().sin_addr.s_addr;
    auto clientA = server.getClientAddr().sin_addr;

    // Check if request ID already in cache
    if (mode == 1 && (cache.find({clientAddr, requestID}) != cache.end())) {
        accessDuplicates(server, clientAddr, requestID);
    } else {
        // Extract essential information to check an account's balance
        std::string name, password;
        int accountID, n;

        n = transform::unmarshalInt(info);              info += INT_SIZE;
        name = transform::unmarshalString(info, n);     info += n;
        std::cout << "Account Name: " << name << std::endl;

        n = transform::unmarshalInt(info);              info += INT_SIZE;
        password = transform::unmarshalString(info, n); info += n;
        std::cout << "Account Password: " << password << std::endl;

        accountID = transform::unmarshalInt(info);      info += INT_SIZE;
        std::cout << "Account ID: " << accountID << std::endl;

        // Check the account's balance through Account Manager
        int bodySize = BASIC_RESPONSE_SIZE;
        int currency;
        float balance;
        bool balanceStatus;
        std::pair<Currency, float> accountDetails;
        std::string err;

        try
        {
            accountDetails = AM.checkAccountBalance(name, password, accountID);
            bodySize += INT_SIZE + FLOAT_SIZE;
            balanceStatus = true;
        }
        catch(const std::invalid_argument& ia)
        {
            balanceStatus = false;
            err = std::string(ia.what());
            bodySize += INT_SIZE + err.size();
            std::cout << err << std::endl;
        }

        // Prepare response message
        char header[HEADER_SIZE];
        char body[bodySize];

        transform::marshalInt(bodySize, header);
        char* buffer = body;

        transform::marshalInt(getResponseID(), buffer);     
        buffer += ID_SIZE;

        if (balanceStatus) {
            // Prepare callback message
            std::string callback;
            std::stringstream ss;
            ss << "Account ID " << accountID << " CHECKED by ";
            ss << UDPServer::getAddressString(clientA);
            callback = ss.str();

            updateSubscribers(server, callback);

            transform::marshalChar(ACK_SUCCESS, buffer);    buffer += ACK_SIZE;
            currency = Account::getCurrencyIndex(accountDetails.first);
            transform::marshalInt(currency, buffer);        buffer += INT_SIZE;
            balance = accountDetails.second;
            transform::marshalFloat(balance, buffer);       buffer += FLOAT_SIZE;  
        } else {
            transform::marshalChar(ACK_FAIL, buffer);       buffer += ACK_SIZE;
            transform::marshalInt(err.size(), buffer);      buffer += INT_SIZE;
            transform::marshalString(err, buffer);          buffer += err.size();
        }

        // Save buffer result into cache
        if (mode == 1) {
            std::string s = std::string(body, bodySize);

            cache[{clientAddr, requestID}] = s;
        }

        answer(server, header, body, bodySize);
    }
}


// Function to handle service: Subscribe to server
/*
    Response structure:
        Header
            * Description *     * Type *        * Size (bytes) *
            Size of body        int             4
        Body
            * Description *     * Type *        * Size (bytes) *
            Response ID         int             4
            Acknowledgement     char            1
*/
void Handler::serviceSubscribe(UDPServer& server, char* info, int requestID) {
    auto clientAddr = server.getClientAddr().sin_addr.s_addr;
    auto clientA = server.getClientAddr();

    // Check if request ID already in cache
    if (mode == 1 && (cache.find({clientAddr, requestID}) != cache.end())) {
        accessDuplicates(server, clientAddr, requestID);
    } else {
        // Extract essential information to subscribe to server
        int durationSec;

        durationSec = transform::unmarshalInt(info);      info += INT_SIZE;
        std::cout << "Client Address: " << UDPServer::getAddressString(clientA.sin_addr) << std::endl;
        std::cout << "Monitor Duration (Sec): " << durationSec << std::endl;

        // Add client to subscribers list
        auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(durationSec * 1000);
        auto epoch = deadline.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
        unsigned long timestamp = value.count();

        monitor.addSubscriber(clientA, timestamp);
        monitor.sortSubscribers();

        // Prepare response message
        int bodySize = BASIC_RESPONSE_SIZE;
        char header[HEADER_SIZE];
        char body[bodySize];

        transform::marshalInt(bodySize, header);
        char* buffer = body;

        transform::marshalInt(getResponseID(), buffer); buffer += ID_SIZE;
        transform::marshalChar(ACK_SUCCESS, buffer);    buffer += ACK_SIZE;

        // Save buffer result into cache
        if (mode == 1) {
            std::string s = std::string(body, bodySize);

            cache[{clientAddr, requestID}] = s;
        }

        answer(server, header, body, bodySize);
    }
}


// Callback function to update all active subscribers
void Handler::updateSubscribers(UDPServer& server, std::string message) {
    // Remove all expired subscribers, if any
    monitor.removeSubscribers();

    auto subscribers = monitor.getSubscribers();
    int bodySize = message.size();
    char header[HEADER_SIZE];
    char body[bodySize];

    transform::marshalInt(bodySize, header);

    for (int i = 0; i < bodySize; i++) {
        body[i] = message[i];
    }

    std::cout << "Callback: " << message << std::endl;

    for (const auto& pair : subscribers) {
        sockaddr_in clientAddr = pair.first;

        notify(server, header, body, bodySize, clientAddr);
        std::cout << "Subscriber Address: " << UDPServer::getAddressString(clientAddr.sin_addr) << std::endl;
    }
}
