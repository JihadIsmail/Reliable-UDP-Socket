#ifndef R_UDP_H
#define R_UDP_H

#include "../alarms_package/Alarm_listner.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string>

using namespace std;

/**
*
*/
class R_UDP : Alarm_listner{
    private:
        int udp_socketfd;
        int plp = 0;
    public:
        // Server Constructor
        R_UDP(int port) {

        };

        // Server Constructor for simulation
        R_UDP(int port, int plp) : plp(plp) {};

        // Client Constructor
        R_UDP(string host_name, int port) {

        };

        // Client Constructor for simulation
        R_UDP(string host_name, int port, int plp) : plp(plp) {};

        // Reliable send method
        virtual void send(char* data, int data_size) = 0;
        // Reliable receive method
        virtual void receive(char* data) = 0;
        virtual void close() = 0;

        virtual void on_timeout(int alarm_id) = 0;
};

#endif // R_UDP_H
