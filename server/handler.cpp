#include "handler.h"

using namespace constants;

const int BASIC_RESPONSE_SIZE = ID_SIZE + ACK_SIZE + INT_SIZE;

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
            serviceClose(server);
            break;
        case 3:
            serviceDeposit(server);
            break;
        case 4:
            serviceWithdraw(server);
            break;
        case 5:
            serviceChange(server);
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
    int bodySize = BASIC_RESPONSE_SIZE + FLOAT_SIZE;
    char header[HEADER_SIZE];
    char body[bodySize];

    transform::marshalInt(bodySize, header);
    char* buffer = body;

    transform::marshalInt(getResponseID(), buffer); buffer += ID_SIZE;
    transform::marshalInt(ACK_SUCCESS, buffer);     buffer += ACK_SIZE;
    transform::marshalInt(INT_SIZE, buffer);        buffer += INT_SIZE;
    transform::marshalInt(accountID, buffer);       buffer += INT_SIZE;

    answer(server, header, body);
}