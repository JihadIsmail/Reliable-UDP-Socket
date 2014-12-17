#include "client_simulator.h"

client_simulator::client_simulator() {
    //ctor
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
