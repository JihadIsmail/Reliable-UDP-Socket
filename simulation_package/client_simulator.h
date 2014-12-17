#ifndef CLIENT_SIMULATOR_H
#define CLIENT_SIMULATOR_H

#include "../reliability_package/R_UDP.h"
#include "../reliability_package/stop_and_wait.h"
#include "../reliability_package/go_back_n.h"
#include "../reliability_package/selective_repeat.h"

using namespace std;

const static string STOP_AND_WAIT = "stopAndWait";
const static string GO_BACK_N = "goBackN";
const static string SELECTIVE_REPEAT = "selectiveRepeat";

class client_simulator {
  private:
    R_UDP *socket;

    R_UDP *socket_factory(string reliability_method, string host_name, int port) {
        if(reliability_method == GO_BACK_N)
          return new go_back_n(host_name, port);
        if(reliability_method == SELECTIVE_REPEAT)
          return new selective_repeat(host_name, port);

        return new stop_and_wait(host_name, port);
    };
  public:
    client_simulator(string host_name, int port);

    void request_file(string file_path);
    void receive_file(char* data);

    virtual ~client_simulator();
};

#endif // CLIENT_SIMULATOR_H
