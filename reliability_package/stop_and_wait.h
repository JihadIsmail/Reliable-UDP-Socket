#ifndef STOP_AND_WAIT_H
#define STOP_AND_WAIT_H

#include "R_UDP.h"
#include "../alarms_package/Alarm.h"
#include <mutex>
#include <stdint.h>   /*int*/

/**
* Stop and wait
*/
class stop_and_wait : public R_UDP {
    public:
        // Constructors
        stop_and_wait(int port) : R_UDP(port) {};
        stop_and_wait(int port, int plp) : R_UDP(port, plp) {};
        stop_and_wait(char* host_name, int port) : R_UDP(host_name, port) {};
        stop_and_wait(char* host_name, int port, int plp) : R_UDP(host_name, port, plp) {};

        void send(packet packet);
        void receive(char* data);
        void close();
    private:
        mutex one_pkt_lock;
        Alarm alarm ;
        ack_packet ack = 0;
        packet pkt = 0 ;
        int time_out , bytes_snd_rcv ;
        char* data ;

        void send_ack(uint32_t ackno);



        // Destructor
        virtual ~stop_and_wait();
};

#endif // STOP_AND_WAIT_H
