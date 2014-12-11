#ifndef STOP_AND_WAIT_H
#define STOP_AND_WAIT_H

#include "R_UDP.h"

/**
* Stop and wait
*/
class stop_and_wait : public R_UDP {
    public:
        // Constructors
        stop_and_wait(int port) : R_UDP(port) {};
        stop_and_wait(int port, int plp) : R_UDP(int port, int plp) {};
        stop_and_wait(string host_name, int port) : R_UDP(string host_name, int port) {};
        stop_and_wait(string host_name, int port, int plp) : R_UDP(string host_name, int port, int plp) {};

        void send(char* data, int data_size);
        void receive(char* data);
        void close();

        // Destructor
        virtual ~stop_and_wait();
};

#endif // STOP_AND_WAIT_H
