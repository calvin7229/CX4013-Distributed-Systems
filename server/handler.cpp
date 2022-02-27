#include "handler.h"

using namespace constants;

// Class constructor
Handler::Handler() {
    AM = AccountManager();
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
            serviceOpen(server);
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
