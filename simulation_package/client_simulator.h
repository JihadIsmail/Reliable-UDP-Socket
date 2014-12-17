#ifndef CLIENT_SIMULATOR_H
#define CLIENT_SIMULATOR_H

#include <iostream>

#include "../reliability_package/R_UDP.h"
#include "../reliability_package/stop_and_wait.h"
#include "../reliability_package/go_back_n.h"
#include "../reliability_package/selective_repeat.h"

using namespace std;

class client_simulator {
  private:
    R_UDP *socket;

    R_UDP *socket_factory(string reliability_method, char* host_name, int port) {
        if(reliability_method == "goBackN")
          return new go_back_n(host_name, port);
        if(reliability_method == "selectiveRepeat")
          return new selective_repeat(host_name, port);

        return new stop_and_wait(host_name, port);
    };
  public:
    client_simulator(string reliabilty_method, char* host_name, int port);

    void request_file(string file_path);
    void receive_file(char* data);

    virtual ~client_simulator();
};

#endif // CLIENT_SIMULATOR_H
