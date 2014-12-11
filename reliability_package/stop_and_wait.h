#ifndef STOP_AND_WAIT_H
#define STOP_AND_WAIT_H

#include "R_UDP.h"

/**
*
*/
class stop_and_wait : public R_UDP {
    public:
        // Constructor
        stop_and_wait(int plp) : R_UDP(plp) {};

        void send(char* data);
        void receive(char* data);
        void close();

        // Destructor
        virtual ~stop_and_wait();
};

#endif // STOP_AND_WAIT_H
