#pragma once

#include <iostream>
#include <sstream>
#include <random>
#include <chrono>
#include <map>

#include "accountmanager.h"
#include "monitor.h"
#include "udpserver.h"
#include "transform.h"
#include "constants.h"

class Handler {
    private:
        AccountManager AM;
        Monitor monitor;
        std::mt19937 mt;
        std::uniform_real_distribution<double> distribution;
        
        const double low = 0.0;
        const double high = 1.0;
        std::map<std::pair<unsigned long, int>, std::string> cache;
        int responseID;
        double failureRate;
    
    public:
        Handler(double failureRate);

        int getResponseID();

        void inquire(UDPServer& server);
        void answer(UDPServer& server, char* header, char* body, int bodySize);
        void notify(UDPServer& server, char* header, char* body, int bodySize, sockaddr_in clientAddr);
        void accessDuplicates(UDPServer& server, unsigned long clientAddr, int requestID);
        void serviceOpen(UDPServer& server, char* info, int requestID);
        void serviceClose(UDPServer& server, char* info, int requestID);
        void serviceDeposit(UDPServer& server, char* info, int requestID);
        void serviceWithdraw(UDPServer& server, char* info, int requestID);
        void serviceChange(UDPServer& server, char* info, int requestID);
        void serviceTransfer(UDPServer& server, char* info, int requestID);
        void serviceBalance(UDPServer& server, char* info, int requestID);
        void serviceSubscribe(UDPServer& server, char* info, int requestID);
        void updateSubscribers(UDPServer& server, std::string message);
};