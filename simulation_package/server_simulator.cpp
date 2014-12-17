#include "server_simulator.h"

server_simulator::server_simulator(string reliability_method, int port, int plp) {
  //ctor
  socket = socket_factory(reliability_method, port, plp);
}

void server_simulator::receive_requested_file_path(char *file_path) {
  // TODO(houssainy)implement all the function
  char *data = new char[PACKET_SIZE];
  socket->receive(data);

  cout << "Data Received " << data << endl;
}

void server_simulator::send_file(char* file_path) {
  // TODO(houssainy)implement all the function
  string hello = "helloWorld!";
  socket->r_send(hello.c_str(), hello.size());
}

server_simulator::~server_simulator() {
  //dtor
  socket->r_close();
  delete socket;
}
