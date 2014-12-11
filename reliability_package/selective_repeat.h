#ifndef SELECTIVE_REPEAT_H
#define SELECTIVE_REPEAT_H

#include "R_UDP.h"
#include "../congestion_control_package/congestion_controller.h"

/**
*
*/
class selective_repeat : public R_UDP {
    private:
        int cwnd;
        congestion_controller cong_controller;

    public:
        // Constructors
        selective_repeat(int port) : R_UDP(port) {};
        selective_repeat(int port, int plp) : R_UDP(int port, int plp) {};
        selective_repeat(string host_name, int port) : R_UDP(string host_name, int port) {};
        selective_repeat(string host_name, int port, int plp) : R_UDP(string host_name, int port, int plp) {};

        void send(char* data, int data_size);
        void receive(char* data);
        void close();

        // Destructor
        virtual ~selective_repeat();


};

#endif // SELECTIVE_REPEAT_H
