#ifndef ALARM_H
#define ALARM_H

#include "Alarm_listner.h"
#include "chrono"
#include "thread"
using namespace std;

class Alarm {
    private:
        Alarm_listner *listner;
        bool stop_sleep = false;
        thread timer ;
        void wait_for_time(unsigned int time_out, unsigned int alarm_id);
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
