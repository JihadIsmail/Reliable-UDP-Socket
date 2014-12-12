#include "Alarm.h"

void Alarm::start(unsigned int time_out, unsigned int alarm_id) {

}

void Alarm::stop() {

}

Alarm::~Alarm() {
    //dtor
    delete listner;
}
