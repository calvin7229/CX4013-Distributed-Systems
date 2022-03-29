#include <iostream>

#include "handler.h"

/* 
        Main Function
<prog> <port> <mode> <failureRate>

port        : port number
mode        : modes for invocation semantics, 0 for at-least-once and 1 for at-most-once
failureRate : failure rate in percentage
*/
int main(int argc, char* argv[]) {
    int port = 8888;
    int mode = 0;
    double failureRate = 0;

    if (argc >= 2)  port = atoi(argv[1]);
    if (argc >= 3)  mode = atoi(argv[2]);
    if (argc >= 4)  failureRate = ((double)atoi(argv[3])) / (double)100.0;

    UDPServer udp = UDPServer(port);
    Handler hand = Handler(mode, failureRate);

    std::cout << "Main Function Started" << std::endl;

    while (true) {
        hand.inquire(udp);
    }

    return 0;
}