#ifndef SERVER_SIMULATOR_H
#define SERVER_SIMULATOR_H

#include "../reliability_package/R_UDP.h"
#include "../reliability_package/stop_and_wait.h"
#include "../reliability_package/go_back_n.h"
#include "../reliability_package/selective_repeat.h"

using namespace std;

const static string STOP_AND_WAIT = "stopAndWait";
const static string GO_BACK_N = "goBackN";
const static string SELECTIVE_REPEAT = "selectiveRepeat";

class server_simulator {
  private:
    R_UDP *socket;

    R_UDP *socket_factory(string reliability_method, int port, int plp) {
        if(reliability_method == GO_BACK_N)
          return new go_back_n(port, plp);
        if(reliability_method == SELECTIVE_REPEAT)
          return new selective_repeat(port, plp);

        return new stop_and_wait(port, plp);
    };
  public:
    server_simulator(string reliability_method, int port, int plp);

    void receive_requested_file_path(char *file_path);
    void send_file(char *file_path);

    virtual ~server_simulator();
};

#endif // SERVER_SIMULATOR_H
