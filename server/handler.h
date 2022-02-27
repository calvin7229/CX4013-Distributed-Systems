#pragma once

#include <iostream>

#include "accountmanager.h"
#include "udpserver.h"
#include "transform.h"
#include "constants.h"

class Handler {
    private:
        AccountManager AM;

    
    public:
        Handler();

        void inquire(UDPServer& server);
        void answer(UDPServer& server, char* header, char* body);
        void serviceOpen(UDPServer& server);
        void serviceClose(UDPServer& server);
        void serviceDeposit(UDPServer& server);
        void serviceWithdraw(UDPServer& server);
        void serviceChange(UDPServer& server);
};