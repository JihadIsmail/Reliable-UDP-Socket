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
        go_back_n(int port, int plp) : R_UDP(int port, int plp) {};
        go_back_n(string host_name, int port) : R_UDP(string host_name, int port) {};
        go_back_n(string host_name, int port, int plp) : R_UDP(string host_name, int port, int plp) {};

        void send(char* data, int data_size);
        void receive(char* data);
        void close();

        // Destructor
        virtual ~go_back_n();
};

#endif // GO_BACK_N_H
