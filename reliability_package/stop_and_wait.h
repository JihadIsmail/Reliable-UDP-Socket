#ifndef STOP_AND_WAIT_H
#define STOP_AND_WAIT_H

#include "R_UDP.h"
#include "../alarms_package/Alarm.h"

#include <mutex>

#include <stdio.h>
#include <stdlib.h> /* exit function */
#include <string.h> /* bzero */
#include <unistd.h>
#include <sys/types.h> /* socket */
#include <sys/socket.h> /* socket */
#include <netdb.h>
#include <netinet/in.h> /* socket */
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

        void r_send(packet packet);
        void receive(char* data);
        void close();
    private:
        mutex one_pkt_lock;
       // Alarm alarm() ;
        ack_packet ack;
        packet pkt;
        int time_out , bytes_snd_rcv ;
        char* data ;

        void send_ack(uint32_t ackno);

        // Destructor
        virtual ~stop_and_wait();
};

#endif // STOP_AND_WAIT_H
