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
        // Constructor
        selective_repeat(int plp) : R_UDP(plp) {};

        void send(char* data);
        void receive(char* data);
        void close();

        // Destructor
        virtual ~selective_repeat();


};

#endif // SELECTIVE_REPEAT_H
