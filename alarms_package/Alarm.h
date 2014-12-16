#ifndef ALARM_H
#define ALARM_H

#include "Alarm_listner.h"

#include "chrono"
#include "thread"

#include <iostream>
using namespace std;

class Alarm {
    private:
      Alarm_listner *listner;

      bool is_stopped = false;
      thread timer;

      void sleep(unsigned int time_out, unsigned int alarm_id);
    public:
      Alarm() {};

      void set_alarm_listner(Alarm_listner *listner) {
          this->listner = listner;
      }

      void start(unsigned int time_out, unsigned int alarm_id);
      void stop();

      virtual ~Alarm();
};

#endif // ALARM_H
