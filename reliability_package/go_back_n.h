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
        // Constructor
        go_back_n(int plp) : R_UDP(plp) {};

        void send(char* data);
        void receive(char* data);
        void close();

        // Destructor
        virtual ~go_back_n();
};

#endif // GO_BACK_N_H
