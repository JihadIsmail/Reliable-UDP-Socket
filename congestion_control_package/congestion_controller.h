/**
 * congestion_controller is an implementation for congestion control mechanism
 *
 * Implementation Summary:
 *  - cwnd is initiated by 1
 *  - cwnd is doubled everty RTT until it reaches the ssthresh "Slow Start".
 *  - When ssthresh is reached cwnd is increased by 1 every RTT
 *    "Congestion avoidance".
 *  - On timeout:
 *    = cwnd = 1
 *    = ssthresh = cwnd/2
 *    = then start grow exponentially again.
 */
#ifndef CONGESTION_CONTROLLER_H
#define CONGESTION_CONTROLLER_H

#include <iostream>

using namespace std;

class congestion_controller {
  private:
    /** Constants **/
    static const short SLOW_START = 1;
    static const short CONGESTION_AVOIDANCE = 2;

    short cuurent_state = SLOW_START;

    int cwnd = 1; // suggested window size.

    int ssthresh = 65536; // threshould to switch from slow start state to
                          // congestion avoidance state.
  public:
    congestion_controller() {};

    int get_cwnd() {
      return cwnd;
    }

    void on_rtt_passed() {
      switch(cuurent_state) {
        case SLOW_START:
          cwnd *= 2;
          if(cwnd >= ssthresh) {
            cuurent_state = CONGESTION_AVOIDANCE;

            cout << "CONGESTION_CONTROLLER: Congestion Avoidance "<<
                    "state started..." << endl;
          }
          break;
        case CONGESTION_AVOIDANCE:
          cwnd++;
          break;
      }
    }

    void on_time_out() {
      ssthresh = cwnd/2;
      cwnd = 1;
      cuurent_state = SLOW_START;

      cout << "CONGESTION_CONTROLLER: Timeout occured and slow start " <<
              "state started again." << endl;
    }
};

#endif // CONGESTION_CONTROLLER_H
