#pragma once

#include <iostream>

#include "accountmanager.h"
#include "udpserver.h"
#include "transform.h"
#include "constants.h"

class Handler {
    private:
        AccountManager AM;
        int responseID;
    
    public:
        Handler();

        int getResponseID();

        void inquire(UDPServer& server);
        void answer(UDPServer& server, char* header, char* body);
        void serviceOpen(UDPServer& server, char* info, int requestID);
        void serviceClose(UDPServer& server, char* info, int requestID);
        void serviceDeposit(UDPServer& server, char* info, int requestID);
        void serviceWithdraw(UDPServer& server, char* info, int requestID);
        void serviceChange(UDPServer& server, char* info, int requestID);
};