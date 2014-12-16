#include "go_back_n.h"

void go_back_n::r_send() {

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
		packet pckt = packets[current-deleted_pckts];

		if (pckt.seqno < base + cwnd ){
            char* data = new char[sizeof(pckt)];
            memcpy(data, &pckt, sizeof(pckt));
            int	bytes_snd_rcv = send(udp_socketfd, data, sizeof(pckt),0);
            if (bytes_snd_rcv == -1) {
                cout << "Go_Back_N: ERROR sending pkt!" << endl;
                exit(0);
            }
            current++;
            if (pckt.seqno == base)
                alarm.start(time_out,pckt.seqno);
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
            if (base ==  ack.ackno)
                alarm.stop();
            else
                alarm.start(time_out ,ack.ackno+1);
            }
            base = ack.ackno+1;
            while (base-deleted_pckts > 0){
                packets.pop_front();
                deleted_pckts++;
            }
            if (pause)
                cv.notify_all();

    }
}

void go_back_n::receive(char* data) {
    packet pckt;
    int bytes_snd_rcv = recv(udp_socketfd, data, sizeof(data), 0);
    if (bytes_snd_rcv == -1) {
        cout << "stop_and_wait:: ERROR receiving data." << endl;
        exit(0);
    }
    if (pckt.seqno == current){
        ack_packet ack ;
        ack.len = sizeof(ack);
        ack.ackno = pckt.seqno;
        char* ack_data = new char[sizeof(ack)];
        memcpy(ack_data, &ack, sizeof(ack));
        int	bytes_snd_rcv = send(udp_socketfd, ack_data, sizeof(ack),0);
        if (bytes_snd_rcv == -1) {
            cout << "Go_Back_N: ERROR sending ack!" << endl;
            exit(0);
        }
        current++;
    }
    else{
        if (current>0){
            ack_packet ack ;
            ack.len = sizeof(ack);
            ack.ackno = current-1;
            char* ack_data = new char[sizeof(ack)];
            memcpy(ack_data, &ack, sizeof(ack));
            int	bytes_snd_rcv = send(udp_socketfd, ack_data, sizeof(ack),0);
            if (bytes_snd_rcv == -1) {
                cout << "Go_Back_N: ERROR sending ack!" << endl;
                exit(0);
            }
        }
    }
}

void go_back_n::on_timeout(int alarm_id){
        current = base;
        if (pause)
            cv.notify_all();
}

void go_back_n::r_close() {

    stop = true;
    if (pause)
        cv.notify_all();
    close(udp_socketfd);
   // thd.join();
}
go_back_n::~go_back_n() {

}
