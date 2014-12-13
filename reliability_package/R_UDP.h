#ifndef R_UDP_H
#define R_UDP_H

#include "../alarms_package/Alarm_listner.h"

#include <stdlib.h> /* exit function */
#include <string.h> /* bzero */
#include <unistd.h>
#include <sys/types.h> /* socket */
#include <sys/socket.h> /* socket */
#include <netdb.h>
#include <netinet/in.h> /* socket */
#include <stdint.h>   /*int*/
#include <math.h>       /* ceil */
#include <iostream>

using namespace std;

/**
*
*/
class R_UDP : Alarm_listner{
    public:
        // Server Constructor
        R_UDP(int port) {
            create_udp_server(port);
        };

        // Server Constructor for simulation
        R_UDP(int port, int plp) : plp(plp) {
            create_udp_server(port);
        };

        // Client Constructor
        R_UDP(char* host_name, int port) {
            create_udp_client(host_name, port);
        };

        // Client Constructor for simulation
        R_UDP(char* host_name, int port, int plp) : plp(plp) {
            create_udp_client(host_name, port);
        };

        // Reliable send method
        virtual void send(char* data, int data_size) = 0;
        // Reliable receive method
        virtual void receive(char* data) = 0;
        virtual void close() = 0;

        virtual void on_timeout(int alarm_id) = 0;

    private:
        int udp_socketfd;
        int plp = 0;
        uint16_t chksum;

        /* Data-only packets 512 byte */
        struct packet {
        /* Header */
        uint16_t chksum;
        uint16_t len;
        uint32_t seqno;
        char data[500]; /* Data */
        };

        /* Ack-only packets are only 8 bytes */
        struct ack_packet {
        uint16_t chksum;
        uint16_t len;
        uint32_t ackno;
        };



        void create_udp_server(int port) {
            udp_socketfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (udp_socketfd < 0) {
                cout<< "Error in opening socket!" << endl;
                exit(EXIT_FAILURE);
            }

            struct sockaddr_in serv_addr, cli_addr;
            socklen_t clilen;

            // Initialize server configuration.
            bzero((char *) &serv_addr, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = INADDR_ANY;
            serv_addr.sin_port = htons(port);

            if (bind(udp_socketfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) {
                cout<< "Error on binding port " << port << endl;
                exit(EXIT_FAILURE);
            }
        }

        void create_udp_client(char* host_name, int port) {
            udp_socketfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (udp_socketfd < 0) {
              cout << "Can't open Socket!" << endl;
              exit(EXIT_FAILURE);
            }

            struct hostent *server = gethostbyname(host_name);
            if (server == NULL) {
              cout << "ERROR: no such host!" << endl;
              exit(EXIT_FAILURE);
            }

            struct sockaddr_in serv_addr;
            bzero((char *) &serv_addr, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            bcopy((char *)server->h_addr,
               (char *)&serv_addr.sin_addr.s_addr,
               server->h_length);
            serv_addr.sin_port = htons(port);
        }
};

#endif // R_UDP_H
