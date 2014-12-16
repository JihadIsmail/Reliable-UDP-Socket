#include "stop_and_wait.h"

void stop_and_wait::r_send() {

	// TODO
	//  use "struct" for packet to packeting the data into
	//  into data packets and send it one by one
	// wait to the "ack" packet to send the nxt data packet
	//start a timer after each pckt`s sending
	// if the time expired retransmit the pkt again
	// if ack received before the timeout expired snd the nxt packet
	Alarm alarm;
	unique_lock<std::mutex> lck(mtx);
	while (!stop){
        // get new CWND from congestion control
		if(packets.empty()){
			pause = true;
			cout<<"zzz..."<<" ,queue size: " <<packets.size()<<endl;
			cv.wait(lck);
            if (stop)
                break;
		}
		pause =false;
        pkt = packets.front();

        char* data = new char[sizeof(pkt)];
        memcpy(data, &pkt, sizeof(pkt));
        alarm.start(time_out,pkt.seqno);
        int	bytes_snd_rcv = send(udp_socketfd, data, sizeof(pkt),0);
        if (bytes_snd_rcv == -1) {
            cout << "stop_and_wait: ERROR sending pkt!" << endl;
            exit(0);
        }
        // Restransmit cases:
        // - received and (corrupted or wrong seq number)
        // 	 ACTION:
        // 		No action, just wait for the ack again.
        // - timeout
        // 	 ACTION:
        //		retransmit and start timer again.
        // Normal case:
        // - received ( And noot corrupted and right seq number)
        // 	 ACTION:
        //		stop timer

        bool ack_corrupted = false;
        do{
            bytes_snd_rcv = recv(udp_socketfd, data, sizeof(data), 0);
            if (bytes_snd_rcv == -1) {
                cout << "stop_and_wait:: ERROR receiving Ack." << endl;
                exit(0);
            }
            memcpy(&ack, data, sizeof(ack));

            if(ack.len != 8 || (ack.ackno != pkt.seqno && ack.ackno != pkt.seqno -1 ) ) { // corrupted
                ack_corrupted = true;
                continue;
            }
            //checksum ack
            // TODO
            //===========
            //if (chksum != ack.chksum) {
            //  ack_corrupted = true;
            //  continue;
            //}
            else if(ack.ackno == pkt.seqno){
                ack_corrupted = false ;
                packets.pop_front();
                alarm.stop();
                break ;
            }
            else if(ack.ackno == pkt.seqno -1 )
            break ;
        } while(ack_corrupted && !stop);
    }
}

void stop_and_wait::receive(char* data) {
	// TODO
    int bytes_snd_rcv = recv(udp_socketfd, data, sizeof(data), 0);
    if (bytes_snd_rcv == -1) {
        cout << "stop_and_wait:: ERROR receiving data." << endl;
        exit(0);
    }
    memcpy(&pkt, data, sizeof(pkt));

    if (pkt.len <= 512) {
		//checksum of pkt sent
		//=======
		//if (chksum == pkt.chksum) {
		//	send_ack(pkt.seqno);
		//}
		if (pkt.seqno == current){
			send_ack(pkt.seqno);
			current++;
		}
		else{
            send_ack(current-1);
		}
	}

}

void stop_and_wait::r_close() {
	// TODO
	stop=true;
	close(udp_socketfd);
	cout <<"connection closed."<<endl;
}
void stop_and_wait::send_ack(uint32_t ackno) {
	ack.len = 8;
	//checksum of ack which will be sent
	//==========
	//ack.chksum = chksum;

    char* data = new char[sizeof(ack)];
	memcpy(data, &ack, sizeof(ack));
	bytes_snd_rcv = send(udp_socketfd, data, sizeof(data), 0);
	if(bytes_snd_rcv == -1)
		cout <<"stop_and_wait::send_ack : ERROR sending ack"<<endl;
}

void stop_and_wait::on_timeout(int alarm_id) {
	// TODO
	if(alarm_id == pkt.seqno){
        stop = true;
	}
}

stop_and_wait::~stop_and_wait() {

}
