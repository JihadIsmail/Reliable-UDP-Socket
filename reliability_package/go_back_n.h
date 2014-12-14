#ifndef GO_BACK_N_H
#define GO_BACK_N_H

#include "R_UDP.h"
#include "../congestion_control_package/congestion_controller.h"

/**
*
*/
class go_back_n : public R_UDP {
    private:
        int cwnd;
        congestion_controller cong_controller;

    public:
        // Constructors
        go_back_n(int port) : R_UDP(port) {};
        go_back_n(int port, int plp) : R_UDP(port, plp) {};
        go_back_n(char* host_name, int port) : R_UDP(host_name,port) {};
        go_back_n(char*host_name, int port, int plp) : R_UDP(host_name, port, plp) {};

        void send(packet packet);
        void receive(char* data);
        void close();

        // Destructor
        virtual ~go_back_n();
};

#endif // GO_BACK_N_H
