#pragma once

#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/time.h> 
#include <arpa/inet.h>

class UDPServer {
    private:
        int sockfd;         // File descriptor for the socket object
        int port;           // Port number to listen on
        size_t clientLen;   // Client's address size in number of bytes

        // Stucture describing an Internet socket address
        sockaddr_in serverAddr;      // Details related to server's address      
        sockaddr_in clientAddr;      // Details related to client's address

    public:
        UDPServer(int port);

        int receive(char* buffer, size_t bufferSize, int timeoutSecs);
        void send(const char* buffer, size_t bufferSize);
        void send(const char* buffer, size_t bufferSize, sockaddr_in clientAddr);

        sockaddr_in getClientAddr();
        size_t getClientLen();
        static std::string getAddressString(in_addr address);
};