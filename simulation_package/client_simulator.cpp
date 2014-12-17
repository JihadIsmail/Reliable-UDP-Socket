#include "client_simulator.h"

client_simulator::client_simulator(string reliability_method, char* host_name, int port) {
  //ctor
  socket = socket_factory(reliability_method, host_name, port);
}

void client_simulator::request_file(string file_path) {
  // TODO(houssainy) implement all the function
  socket->r_send(file_path.c_str(), file_path.size());
}

void client_simulator::receive_file(char* data) {
  // TODO(houssainy) implement all the function
  socket->receive(data);

  cout << "Data Received " << data << endl;
}

client_simulator::~client_simulator() {
  //dtor
  socket->r_close();
  delete socket;
}
