#ifndef GO_BACK_N_H
#define GO_BACK_N_H

#include "R_UDP.h"
#include <mutex>
#include "../congestion_control_package/congestion_controller.h"
#include "../alarms_package/Alarm.h"

using namespace std;
/**
*
*/
class go_back_n : public R_UDP {
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
        go_back_n(int port) : R_UDP(port) {};
        go_back_n(int port, int plp) : R_UDP(port, plp) {};
        go_back_n(char* host_name, int port) : R_UDP(host_name,port) {};

        void r_send();
        void receive(char* data);
        void ack_receive(char* data);
        void r_close();


        // Destructor
        virtual ~go_back_n();
};

#endif // GO_BACK_N_H
