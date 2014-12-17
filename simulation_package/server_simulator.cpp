#include "server_simulator.h"

server_simulator::server_simulator() {
    //ctor
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
