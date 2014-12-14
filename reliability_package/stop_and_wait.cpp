#include "stop_and_wait.h"

void stop_and_wait::send(char* data, int data_size) {
	// TODO



/*	//  use "struct" for packet to packeting the data into

	//  into data packets and send it one by one

	// wait to the "ack" packet to send the nxt data packet

	//start a timer after each pckt`s sending
	// if the time expired retransmit the pkt again
	// if ack received before the timeout expired snd the nxt packet

	int index = 0;
	int seq_num = 0;
	int len = 0;
	packet pkt;
	char * pkt_data;
//	pkt.chksum = ;

	while (index < data_size) {
		int i = 0;
		len = data_size - index; // remained length
		if ((len) <= 500) {

			while (index < data_size) {
				pkt.data[i] = data[index];
				i++, index++;
			}
			pkt.len = len + 12;
			pkt.seqno = seq_num;
	//do checksum part
	 //TODO
	//=============
	 pkt.chksum = chksum;
			memcpy(pkt_data, &pkt, sizeof(pkt));
			// start timer and wait for it
				 //TODO
				//========
			int bytes_sent = send(udp_socketfd, pkt_data, sizeof(pkt), 0);
			if (bytes_sent == -1)
				cout << "error sending pkt" << endl;
		bytes_sent = recieve(udp_socketfd, pkt_data, sizeof(pkt), 0);
			if(bytes_sent == -1)
			cout<<"error recieving ack"<<endl;

							seq_num++;
//===================
		} else {
			while (i < 500) {
				pkt.data[i] = data[index];
				i++, index++;
			}
			pkt.len = 512;
			pkt.seqno = seq_num;
			seq_num++;

		}
	}*/
}

void stop_and_wait::receive(char* data /*this is a char[512] or char[8]*/) {
	ack_packet ack;
	packet pkt_data;
	memcpy(&pkt_data, data, sizeof(data));
	// do checksum part
	// TODO
	//==============

	// send ack or not

	if (chksum == pkt_data.chksum) {
		ack.len = 8;
		ack.ackno = (pkt_data.seqno) + 1;
		// do ack checksum
		// TODO
		//========
		ack.chksum = chksum;
	//	data = new char [8];
		memcpy(data, &ack, sizeof(ack));
		int byets_sent = send(udp_socketfd, data, sizeof(data), 0);
		if (byets_sent == -1)
			cout << "error ending ack" << endl;
	} else {
		// if data `s corrupted send an ack with the same seq#
		// of data recieved.
		ack.len = 8;
		ack.ackno = pkt_data.seqno;
		// do ack checksum
		// TODO
		//========
		ack.chksum = chksum;
		memcpy(data, &ack, sizeof(ack));
		int byets_sent = send(udp_socketfd, data, sizeof(data), 0);
		if (byets_sent == -1)
			cout << "error ending ack" << endl;
	}
}

void stop_and_wait::close() {
	// TODO
}

stop_and_wait::~stop_and_wait() {

}
