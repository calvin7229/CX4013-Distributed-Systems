#include <udpserver.h>

// Class constructor
UDPServer::UDPServer(int port) {
    /*
        Arguments:
            domain   –  Specifies the communication (AF_INET for IPv4 / AF_INET6 for IPv6) 
            type     –  Type of socket to be created (SOCK_STREAM for TCP / SOCK_DGRAM for UDP) 
            protocol –  Protocol to be used by the socket (0 means use default protocol for the address family)
        
        Returns a file descriptor for the new socket, or -1 for errors
    */
    this->sockfd = socket(AF_INET, SOCK_DGRAM, 0);      
    this->port = port;

    if (this->sockfd < 0) {
        std::cerr << ("Error: Unable To Open Socket.") << std::endl;
        return;
    }

    // Set memory and initialize to zero for both server and client's address DS
    std::memset(&this->serverAddr, 0, sizeof(this->serverAddr));
    std::memset(&this->clientAddr, 0, sizeof(this->clientAddr));

    // Filling server information
    this->serverAddr.sin_family = AF_INET;
    /*
        htonl(): 32-bit number in host byte order -> 32-bit number in network byte order
        htons(): 16-bit number in host byte order -> 16-bit number in network byte order 

        The network byte order is defined to always be big-endian, which may differ from 
        the host byte order on a particular machine. Using network byte ordering for data 
        exchanged between hosts allows hosts using different architectures to exchange address 
        information without confusion because of byte ordering.
    */
    this->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->serverAddr.sin_port = htons((unsigned short)this->port);

    // Bind the socket FD with the server address (which is LEN bytes long)
    int bindStatus = bind(this->sockfd, (const sockaddr*)&this->serverAddr, sizeof(this->serverAddr));

    if (bindStatus < 0) {
        std::cerr << ("Error: Unable To Bind Socket To Server Address") << std::endl;
        return;
    }

    this->clientLen = sizeof(this->clientAddr);
}

// Function to receive message on the server socket through UDP
int UDPServer::receive(char* buffer, size_t bufferSize, int timeoutSecs) /* throw(std::string) */ {
    timeval tv = {.tv_sec = timeoutSecs, .tv_usec = 0};

    /*
        setsockopt:     Control socket behaviour, including allocate buffer space, 
                        control timeouts, or permit socket data broadcasts
        Here, we set option SO_RCVTIMEO to specify timeout for the receive operation
    */
    setsockopt(this->sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    // Read bufferSize bytes into buffer through socket FD
    int n = recvfrom(this->sockfd, buffer, bufferSize, 0, (sockaddr*)&this->clientAddr, (socklen_t*)&this->clientLen);

    if (n < 0) {
        throw std::string("Error: Unable To Receive Message From Client");
    } else if (n == 0) {
        throw std::string("Error: No Message Received From Client");
    } else {
        std::cout << "Server Received " << n << " Bytes Of Data From Client" << std::endl;
    }

    return n;
}

// Function to send message on the server socket through UDP
void UDPServer::send(const char* buffer, size_t bufferSize) {
    // Send bufferSize bytes of buffer on socket FD to peer at address clientAddr
    sendto(this->sockfd, buffer, bufferSize, 0, (sockaddr*)&this->clientAddr, (socklen_t)this->clientLen);
}

// Getter: clientAddr
sockaddr_in UDPServer::getClientAddr() {
    return this->clientAddr;
}

// Getter: clientLen
size_t UDPServer::getClientLen() {
    return this->clientLen;
}