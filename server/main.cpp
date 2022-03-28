#include <iostream>

#include "handler.h"

/* 
        Main Function
<prog> <port> <failureRate>
*/
int main(int argc, char* argv[]) {
    int port = 8888;
    double failureRate = 0;

    if (argc >= 2)  port = atoi(argv[1]);
    if (argc >= 3)  failureRate = ((double)atoi(argv[2])) / (double)100.0;

    UDPServer udp = UDPServer(port);
    Handler hand = Handler(failureRate);

    std::cout << "Main Function Started" << std::endl;

    while (true) {
        hand.inquire(udp);
    }

    return 0;
}