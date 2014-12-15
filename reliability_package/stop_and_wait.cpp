#include "stop_and_wait.h"

void stop_and_wait::r_send(packet pkt) {

	// TODO
	//  use "struct" for packet to packeting the data into
	//  into data packets and send it one by one
	// wait to the "ack" packet to send the nxt data packet
	//start a timer after each pckt`s sending
	// if the time expired retransmit the pkt again
	// if ack received before the timeout expired snd the nxt packet

	one_pkt_lock.lock();
	this->pkt = pkt;
	data = new char[512];
	Alarm alarm ;
	alarm.set_alarm_listner(this);
	alarm.start(time_out, pkt.seqno);
	memcpy(data, &pkt, sizeof(pkt));
	bytes_snd_rcv = send(udp_socketfd, data, sizeof(data),0);
	if (bytes_snd_rcv == -1)
		cout << "stop_and_wait:: ERROR sending pkt." << endl;
	bytes_snd_rcv = recv(udp_socketfd, data, sizeof(data), 0);
	if (bytes_snd_rcv == -1)
		cout << "stop_and_wait:: ERROR receiving Ack." << endl;
	memcpy(&ack, data, sizeof(ack));
	if ((ack.len == 8) || (ack.ackno == pkt.seqno)) {
		//checksum ack
		//===========
		if (chksum == ack.chksum) {
			r_send(pkt);
		}
	}
	one_pkt_lock.unlock();
}

void stop_and_wait::receive(char* data) {
	// TODO
	memcpy(&pkt, data, sizeof(pkt));
	if (pkt.len <= 512) {
		//checksum of pkt sent
		//=======
		if (chksum == pkt.chksum) {
			send_ack(pkt.seqno+1);
		}
		else {
			send_ack(pkt.seqno);

		}
	}

}

void stop_and_wait::close() {
	// TODO
	close();
	cout <<"connection closed."<<endl;
}
void stop_and_wait::send_ack(uint32_t ackno) {
	ack.len = 8;
	//checksum of ack which will be sent
	//==========
	ack.chksum = chksum;
	memcpy(data, &ack, sizeof(ack));
	bytes_snd_rcv = send(udp_socketfd, data, sizeof(data), 0);
	if(bytes_snd_rcv == -1)
		cout <<"stop_and_wait::send_ack : ERROR sending ack"<<endl;
}
void R_UDP::on_timeout(int alarm_id) {
	// TODO
	if(alarm_id == pkt.seqno){
        r_send (pkt);
	}
}

stop_and_wait::~stop_and_wait() {

}
