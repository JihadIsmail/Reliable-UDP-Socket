#include "stop_and_wait.h"

stop_and_wait::stop_and_wait(int port) : R_UDP(port) {
    current = 0;
}

stop_and_wait::stop_and_wait(int port, int plp) : R_UDP(port, plp){
    current = 0;
}

stop_and_wait::stop_and_wait(char* host_name, int port) : R_UDP(host_name, port){
    send_thread = std::thread(&stop_and_wait::r_send, this);
    stop = false;
}

void stop_and_wait::r_send() {
  // TODO
  //  use "struct" for packet to packeting the data into
  //  into data packets and send it one by one
  // wait to the "ack" packet to send the nxt data packet
  //start a timer after each pckt`s sending
  // if the time expired retransmit the pkt again
  // if ack received before the timeout expired snd the nxt packet
  alarm.set_alarm_listner(this);
  unique_lock<std::mutex> lck(mtx);
  while (!stop){
    // TODO() get new CWND from congestion control
    if(packets.empty()) { // No packets are ready to be sent
      pause = true;
      cout << "zzz..." << ", queue size: " << packets.size() << endl;
      cv.wait(lck);
      if(stop)
        break;
    }
    pause = false;
    pkt = packets.front();

    char data[sizeof(pkt)];
    memcpy(data, &pkt, sizeof(pkt));

    //alarm.start(time_out,pkt.seqno);
    cout<<"send data seq number:"<<pkt.seqno<<endl;
    int bytes_snd_rcv = sendto(udp_socketfd, data, sizeof(data), 0,
                      (struct sockaddr *)&rem_addr, sizeof(rem_addr));
    if(bytes_snd_rcv == -1) {
      cout << "Error in sending packet!" << endl;
      exit(0);
    }

    /** Restransmit cases:
     * - received and (corrupted or wrong seq number)
     * 	 ACTION:
     * 		No action, just wait for the ack again.
     * - timeout
     * 	 ACTION:
     *		retransmit and start timer again.
     * Normal case:
     * - received ( And noot corrupted and right seq number)
     * 	 ACTION:
     *		stop timer
     */
    bool ack_corrupted = false;

    char buf[sizeof(ack)];
    do {
      socklen_t addrlen = sizeof(rem_addr);
      bytes_snd_rcv = recvfrom(udp_socketfd, buf, sizeof(ack), 0,
                              (struct sockaddr *)&rem_addr, &addrlen);
      if (bytes_snd_rcv > 1)
        buf[bytes_snd_rcv] = 0; // set last byte 0

      memcpy(&ack, buf, sizeof(ack));
      if(ack.len != 8 || (ack.ackno != pkt.seqno && ack.ackno != pkt.seqno - 1)) { // corrupted
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
      else if(ack.ackno == pkt.seqno) {
        ack_corrupted = false ;
        packets.pop_front();
        alarm.stop();
        break ;
      } else if(ack.ackno == pkt.seqno -1) {
        break;
      }
    } while(ack_corrupted && !stop);
  }
}

void stop_and_wait::receive(char* data) {
  // TODO
  socklen_t addrlen = sizeof(rem_addr);
  bytes_snd_rcv = recvfrom(udp_socketfd, data, sizeof(pkt), 0,
                          (struct sockaddr *)&rem_addr, &addrlen);

  if (bytes_snd_rcv > 1)
    data[bytes_snd_rcv] = 0; // set last byte 0

  memcpy(&pkt, data, sizeof(pkt));

  if (pkt.len <= 512) {
    //checksum of pkt sent
    //=======
    //if (chksum == pkt.chksum) {
    //	send_ack(pkt.seqno);
    //}
    cout<<pkt.seqno<<endl;
    if(pkt.seqno == current) {
      send_ack(pkt.seqno);
      current++;
    } else {
      send_ack(current-1);
    }
  }
}

void stop_and_wait::r_close() {
  // TODO
  //stop=true;
  //close(udp_socketfd);
  send_thread.join();
  cout <<"Socket closed."<<endl;
}

void stop_and_wait::send_ack(uint32_t ackno) {
  ack.len = 8;
  //checksum of ack which will be sent
  //==========
  //ack.chksum = chksum;

  char data[sizeof(ack)];
  memcpy(data, &ack, sizeof(ack));

  bytes_snd_rcv = sendto(udp_socketfd, data, sizeof(data), 0,
                        (struct sockaddr *)&rem_addr, sizeof(rem_addr));
  if(bytes_snd_rcv == -1)
    cout <<"stop_and_wait::send_ack : ERROR sending ack"<<endl;
}

void stop_and_wait::on_timeout(int alarm_id) {
  if(alarm_id == pkt.seqno)
    stop = true;
}

stop_and_wait::~stop_and_wait() {
  r_close(); // TODO(houssainy) what if it's already stopped!
}
