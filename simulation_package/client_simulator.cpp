#include "client_simulator.h"

client_simulator::client_simulator(string reliability_method, string host_name, int port) {
    //ctor
    socket = socket_factory(reliability_method, host_name, port);
}

void client_simulator::request_file(string file_path) {

}

void client_simulator::receive_file(char* data) {

}

client_simulator::~client_simulator() {
    //dtor
    socket->r_close();
    delete socket;
}
