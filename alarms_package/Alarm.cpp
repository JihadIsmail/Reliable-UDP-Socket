#include "Alarm.h"

void Alarm::start(unsigned int time_out, unsigned int alarm_id) {
  timer = std::thread(&Alarm::sleep, this, time_out, alarm_id);
}

void Alarm::sleep(unsigned int time_out, unsigned int alarm_id) {
  is_stopped = false;
  // TODO(houssainy) use unblocking sleep method to be able to wake up the
  // thread instead instead of blocking closing the program!
  this_thread::sleep_for(std::chrono::milliseconds(time_out));
  if(listner != NULL && !is_stopped)
    listner->on_timeout(alarm_id);
  cout << "Alarm woked up" << endl;
}

void Alarm::stop() {
  cout << "Alarm stopped" << endl;
	is_stopped = true;
}

Alarm::~Alarm() {
    //dtor
    timer.join();
    delete listner;
}
