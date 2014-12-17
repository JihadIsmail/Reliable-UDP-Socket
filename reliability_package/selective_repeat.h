#ifndef SELECTIVE_REPEAT_H
#define SELECTIVE_REPEAT_H

#include "R_UDP.h"
#include "../congestion_control_package/congestion_controller.h"
#include "../alarms_package/Alarm.h"


/**
*
*/
class selective_repeat : public R_UDP {
    private:
        int cwnd;
        int current = 0;
        int base = 0;
        int deleted_pckts = 0;
        congestion_controller cong_controller;
        mutex mtx;
        bool stop = false;
        Alarm alarm;
        void on_timeout(int alarm_id);

    public:
        // Constructors
        selective_repeat(int port) : R_UDP(port) {};
        selective_repeat(int port, int plp) : R_UDP(port, plp) {};
        selective_repeat(char* host_name, int port) : R_UDP(host_name, port) {};

        void r_send();
        void receive(char* data);
        void ack_receive(char* data);
        void r_close();

        // Destructor
        virtual ~selective_repeat();


};

#endif // SELECTIVE_REPEAT_H
