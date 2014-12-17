#ifndef CLIENT_SIMULATOR_H
#define CLIENT_SIMULATOR_H

#include "../reliability_package/R_UDP.h"
#include "../reliability_package/stop_and_wait.h"
#include "../reliability_package/go_back_n.h"
#include "../reliability_package/selective_repeat.h"

using namespace std;
class client_simulator {
  private:
    R_UDP *socket;
  public:
    client_simulator(string host_name, int port);

    void request_file(string file_path);
    void receive_file(char* data);

    virtual ~client_simulator();
};

#endif // CLIENT_SIMULATOR_H
