#include "server_simulator.h"

server_simulator::server_simulator(int reliability_method, int port, int plp) {
    //ctor
    socket = socket_factory(reliability_method, port, plp);
}

void server_simulator::receive_requested_file_path() {

}

void server_simulator::send_file(char* file_path) {

}

server_simulator::~server_simulator() {
    //dtor
    socket->close();
    delete socket;
}
