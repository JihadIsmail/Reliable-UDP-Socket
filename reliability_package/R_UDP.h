#ifndef R_UDP_H
#define R_UDP_H

/**
*
*/
class R_UDP {
    private:
        int udp_socketfd;
        int plp = 0;
    public:
        // Constructor for simulation
        R_UDP(int plp) : plp(plp) {};

        // Reliable send method
        virtual void send(char* data) = 0;
        // Reliable receive method
        virtual void receive(char* data) = 0;
        virtual void close() = 0;
};

#endif // R_UDP_H
