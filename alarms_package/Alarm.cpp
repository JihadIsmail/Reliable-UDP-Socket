#include "Alarm.h"

void Alarm::start(unsigned int time_out, unsigned int alarm_id) {
//	 thread thrd(wait_for_time ,time_out);
}

void Alarm::stop() {
	stop_sleep = true;
}

void Alarm::wait_for_time(unsigned int time_out, unsigned int alarm_id){
	this_thread::sleep_for(std::chrono::seconds(time_out));
	 if(!stop_sleep){
		 listner->on_timeout(alarm_id);
	 }
}

Alarm::~Alarm() {
    //dtor
    delete listner;
}
