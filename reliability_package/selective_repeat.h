#ifndef SELECTIVE_REPEAT_H
#define SELECTIVE_REPEAT_H

#include "R_UDP.h"
#include "../congestion_control_package/congestion_controller.h"

/**
*
*/
class selective_repeat : public R_UDP {
    private:
        int cwnd = 1;
        congestion_controller cong_controller;

    public:
        // Constructors
        selective_repeat(int port) : R_UDP(port) {};
        selective_repeat(int port, int plp) : R_UDP(port, plp) {};
        selective_repeat(char* host_name, int port) : R_UDP(host_name, port) {};
        selective_repeat(char* host_name, int port, int plp) : R_UDP(host_name, port, plp) {};

        void send(packet packet);
        void receive(char* data);
        void close();

        // Destructor
        virtual ~selective_repeat();


};

#endif // SELECTIVE_REPEAT_H
