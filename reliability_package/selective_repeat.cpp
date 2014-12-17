#include "selective_repeat.h"

void selective_repeat::r_send() {
    // TODO

    unique_lock<std::mutex> lck(mtx);
	while (!stop){
        // get new CWND from congestion control
		if(packets.empty() || current == cwnd + base){
			pause = true;
			cout<<"zzz..."<<" ,queue size: " <<packets.size()<<endl;
			cv.wait(lck);
            if (stop)
                break;
		}
		pause =false;
		packet pckt = packets[current-deleted_pckts]; // strong right

		if (pckt.seqno < base + cwnd ){
            char* data = new char[sizeof(pckt)];
            memcpy(data, &pckt, sizeof(pckt));
            int	bytes_snd_rcv = send(udp_socketfd, data, sizeof(pckt),0);
            if (bytes_snd_rcv == -1) {
                cout << "Go_Back_N: ERROR sending pkt!" << endl;
                exit(0);
            }
            current++;
            alarm.start(time_out,pckt.seqno); // timer for each pkt
        }
	}
}

void go_back_n::ack_receive(char* data){
    while(!stop){
        ack_packet ack;
        int bytes_snd_rcv = recv(udp_socketfd, data, sizeof(data), 0);
		if (bytes_snd_rcv == -1) {
			cout << "stop_and_wait:: ERROR receiving Ack." << endl;
			exit(0);
		}
		memcpy(&ack, data, sizeof(ack));
		if (ack.ackno < base + cwnd && ack.ackno >= base){
           if (base ==  ack.ackno){
                alarm.stop();
                base = ack.ackno+1;  // only update base when the expecting ack comes
                                     // ex: pkt(1) acked , pkt(2) not acked , pkt(3) acked
                                     // since pkt2 ack `s lost "base" should not exceed it,
                                     // to be prepared that the nxt iteration will begin with 
                                     // pkt(2)
                }
             alarm.stop(); // here i got an ack for a higher order pkt than the expecting one,
                           // i wuold just stop pkt`s timer and save its ack
            }
            while (base-deleted_pckts > 0){
                packets.pop_front();
                deleted_pckts++;
            }
            if (pause)
                cv.notify_all();

    }
}
void selective_repeat::receive(char* data) {
    // TODO
      packet pckt;
    int bytes_snd_rcv = recv(udp_socketfd, data, sizeof(data), 0);
    if (bytes_snd_rcv == -1) {
        cout << "stop_and_wait:: ERROR receiving data." << endl;
        exit(0);
    }
    if (pckt.seqno< base + cwnd && pckt.seqno >= base){ // if (pckt.seqno == current){
        if (base ==  pckt.seqno){   // only if the expected data comes will ubdate its base
                                    // otherwise it will only accept and save the data
            base = ack.ackno+1;
        }
        ack_packet ack ;
        ack.len = sizeof(ack);
        ack.ackno = pckt.seqno;
        char* ack_data = new char[sizeof(ack)];
        memcpy(ack_data, &ack, sizeof(ack));
        int bytes_snd_rcv = send(udp_socketfd, ack_data, sizeof(ack),0);
        if (bytes_snd_rcv == -1) {
            cout << "Go_Back_N: ERROR sending ack!" << endl;
            exit(0);
        }
    }
    // no need to send an ack with akno = seqno -1
}

void selective_repeat::on_timeout(int alarm_id){
        current = base;
        if (pause)
            cv.notify_all();
}
void selective_repeat::r_close() {
    // TODO
     stop = true;
    if (pause)
        cv.notify_all();
    close(udp_socketfd);
   // thd.join();
}

selective_repeat::~selective_repeat() {

}
