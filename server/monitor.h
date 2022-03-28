#pragma once

#include <iostream>
#include <algorithm>
#include <deque>
#include <chrono>

#include "udpserver.h"

class Monitor {
    private:
        std::deque<std::pair<sockaddr_in, unsigned long>> subscribers;
    
    public:
        Monitor();

        void addSubscriber(sockaddr_in clientAddr, unsigned long timestamp);
        void removeSubscribers();
        void sortSubscribers();
        std::deque<std::pair<sockaddr_in, unsigned long>> getSubscribers();
};