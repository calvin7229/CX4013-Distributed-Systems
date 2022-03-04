#include "handler.h"

using namespace constants;

const int BASIC_RESPONSE_SIZE = ID_SIZE + ACK_SIZE;

// Class constructor
Handler::Handler() {
    AM = AccountManager();
    responseID = 0;
}

// Getter: responseID
int Handler::getResponseID() {
    return this->responseID++;
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
        receiveStatus = server.receive(body, bodySize, STD_TIMEOUT);
        std::cout << "Body Received!" << std::endl;
    } catch (std::string err) {
        std::cerr << err << std::endl;
        return;
    }

    // Inquire service type
    char* buffer = body;
    int requestID = transform::unmarshalInt(buffer);
    buffer += INT_SIZE;
    int serviceType = transform::unmarshalInt(buffer);
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
            serviceChange(server, buffer, requestID);
            break;
    }
}   

// Function to answer client's request with a response
void Handler::answer(UDPServer& server, char* header, char* body) {
    // Send header
    server.send(header, HEADER_SIZE);

    // Send body
    server.send(body, sizeof(body));
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
            Account ID size     int             4
            Account ID          int             4
*/
void Handler::serviceOpen(UDPServer& server, char* info, int requestID) {
    auto clientAddr = server.getClientAddr().sin_addr.s_addr;

    // Extract essential information to open an account
    std::string name, password;
    Currency currency;
    float balance;
    int n, temp;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    name = transform::unmarshalString(info, n);     info += n;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    password = transform::unmarshalString(info, n); info += n;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    temp = transform::unmarshalInt(info);           info += n;
    currency = Account::getCurrency(temp);

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    balance = transform::unmarshalFloat(info);      info += n;

    // Create the account through Account Manager
    int accountID = AM.createAccount(name, password, currency, balance);

    // Prepare response message
    int bodySize = BASIC_RESPONSE_SIZE + INT_SIZE + INT_SIZE;
    char header[HEADER_SIZE];
    char body[bodySize];

    transform::marshalInt(bodySize, header);
    char* buffer = body;

    transform::marshalInt(getResponseID(), buffer); buffer += ID_SIZE;
    transform::marshalChar(ACK_SUCCESS, buffer);    buffer += ACK_SIZE;
    transform::marshalInt(INT_SIZE, buffer);        buffer += INT_SIZE;
    transform::marshalInt(accountID, buffer);       buffer += INT_SIZE;

    answer(server, header, body);
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

    // Extract essential information to close an account
    std::string name, password;
    int accountID, n;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    name = transform::unmarshalString(info, n);     info += n;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    password = transform::unmarshalString(info, n); info += n;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    accountID = transform::unmarshalInt(info);      info += n;

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
        bodySize += INT_SIZE + sizeof(ia.what());
        closeStatus = false;
        err = std::string(ia.what());
    }

    // Prepare response message
    char header[HEADER_SIZE];
    char body[bodySize];

    transform::marshalInt(bodySize, header);
    char* buffer = body;

    transform::marshalInt(getResponseID(), buffer);     
    buffer += ID_SIZE;

    if (closeStatus) {
        transform::marshalChar(ACK_SUCCESS, buffer);    buffer += ACK_SIZE;
    } else {
        transform::marshalChar(ACK_FAIL, buffer);       buffer += ACK_SIZE;
        transform::marshalInt(err.size(), buffer);      buffer += INT_SIZE;
        transform::marshalString(err, buffer);          buffer += err.size();
    }

    answer(server, header, body);
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
            Currency size       int             4
            Currency            int             4
            Balance size        int             4
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

    // Extract essential information to deposit money into an account
    std::string name, password;
    int accountID, n;
    float amount;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    name = transform::unmarshalString(info, n);     info += n;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    password = transform::unmarshalString(info, n); info += n;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    accountID = transform::unmarshalInt(info);      info += n;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    amount = transform::unmarshalFloat(info);       info += n;

    // Deposit the amount through Account Manager
    int bodySize = BASIC_RESPONSE_SIZE;
    int currency;
    float balance;
    bool depositStatus;
    std::pair<Currency, float> accountDetails;
    std::string err;

    try
    {
        accountDetails = AM.depositMoney(name, password, accountID, amount);
        bodySize += INT_SIZE + INT_SIZE + INT_SIZE + FLOAT_SIZE;
        depositStatus = true;
    }
    catch(const std::invalid_argument& ia)
    {
        bodySize += INT_SIZE + sizeof(ia.what());
        depositStatus = false;
        err = std::string(ia.what());
    }

    // Prepare response message
    char header[HEADER_SIZE];
    char body[bodySize];

    transform::marshalInt(bodySize, header);
    char* buffer = body;

    transform::marshalInt(getResponseID(), buffer);     
    buffer += ID_SIZE;

    if (depositStatus) {
        transform::marshalChar(ACK_SUCCESS, buffer);    buffer += ACK_SIZE;
        
        transform::marshalInt(INT_SIZE, buffer);        buffer += INT_SIZE;
        currency = Account::getCurrencyIndex(accountDetails.first);
        transform::marshalInt(currency, buffer);        buffer += INT_SIZE;

        transform::marshalInt(INT_SIZE, buffer);        buffer += INT_SIZE;
        balance = accountDetails.second;
        transform::marshalFloat(balance, buffer);       buffer += FLOAT_SIZE;  
    } else {
        transform::marshalChar(ACK_FAIL, buffer);       buffer += ACK_SIZE;
        transform::marshalInt(err.size(), buffer);      buffer += INT_SIZE;
        transform::marshalString(err, buffer);          buffer += err.size();
    }

    answer(server, header, body);
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
            Currency size       int             4
            Currency            int             4
            Balance size        int             4
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

    // Extract essential information to withdraw money from an account
    std::string name, password;
    int accountID, n;
    float amount;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    name = transform::unmarshalString(info, n);     info += n;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    password = transform::unmarshalString(info, n); info += n;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    accountID = transform::unmarshalInt(info);      info += n;

    n = transform::unmarshalInt(info);              info += INT_SIZE;
    amount = transform::unmarshalFloat(info);       info += n;

    // Withdraw the amount through Account Manager
    int bodySize = BASIC_RESPONSE_SIZE;
    int currency;
    float balance;
    bool withdrawStatus;
    std::pair<Currency, float> accountDetails;
    std::string err;

    try
    {
        accountDetails = AM.withdrawMoney(name, password, accountID, amount);
        bodySize += INT_SIZE + INT_SIZE + INT_SIZE + FLOAT_SIZE;
        withdrawStatus = true;
    }
    catch(const std::invalid_argument& ia)
    {
        bodySize += INT_SIZE + sizeof(ia.what());
        withdrawStatus = false;
        err = std::string(ia.what());
    }

    // Prepare response message
    char header[HEADER_SIZE];
    char body[bodySize];

    transform::marshalInt(bodySize, header);
    char* buffer = body;

    transform::marshalInt(getResponseID(), buffer);     
    buffer += ID_SIZE;

    if (withdrawStatus) {
        transform::marshalChar(ACK_SUCCESS, buffer);    buffer += ACK_SIZE;
        
        transform::marshalInt(INT_SIZE, buffer);        buffer += INT_SIZE;
        currency = Account::getCurrencyIndex(accountDetails.first);
        transform::marshalInt(currency, buffer);        buffer += INT_SIZE;

        transform::marshalInt(INT_SIZE, buffer);        buffer += INT_SIZE;
        balance = accountDetails.second;
        transform::marshalFloat(balance, buffer);       buffer += FLOAT_SIZE;  
    } else {
        transform::marshalChar(ACK_FAIL, buffer);       buffer += ACK_SIZE;
        transform::marshalInt(err.size(), buffer);      buffer += INT_SIZE;
        transform::marshalString(err, buffer);          buffer += err.size();
    }

    answer(server, header, body);
}
