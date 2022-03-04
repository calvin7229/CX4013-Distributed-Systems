#include <iostream>

#include "handler.h"

int main() {
    int port = 8080;

    UDPServer udp = UDPServer(port);
    Handler hand = Handler();

    while (true) {
        hand.inquire(udp);
    }

    return 0;
}