#include "stop_and_wait.h"

void stop_and_wait::send(char* data, int data_size) {
    // TODO
	//  use "struct" for packet to packeting the data into
		//  into data packets and send it one by one

		// wait to the "ack" packet to send the nxt data packet

		//start a timer after each pckt`s sending
		// if the time expired retransmit the pkt again
		// if ack received before the timeout expired snd the nxt packet}

void stop_and_wait::receive(char* data) {
    // TODO
}

void stop_and_wait::close() {
    // TODO
}

stop_and_wait::~stop_and_wait() {

}
