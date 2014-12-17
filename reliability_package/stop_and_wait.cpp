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
  unique_lock<std::mutex> lck(mtx);
  while (!stop){
    Alarm alarm ;
    alarm.set_alarm_listner(this);
    // TODO() get new CWND from congestion control
    Alarm alarm;
    alarm.set_alarm_listner(this);
    if(packets.empty()) { // No packets are ready to be sent
      pause = true;
      cout << "zzz..." << ", queue size: " << packets.size() << endl;
      cv.wait(lck);
      if(stop)
        break;
    }
    pause = false;

    /** Sending Packet **/
    pkt = packets.front();

    char data[PACKET_SIZE];
    memcpy(data, &pkt, PACKET_SIZE);

    alarm.start(time_out, pkt.seqno);

    int bytes_snd_rcv = sendto(udp_socketfd, data, PACKET_SIZE, 0,
                        (struct sockaddr *)&rem_addr, sizeof(rem_addr));
    if(bytes_snd_rcv == -1) {
      cout << "Error in sending packet!" << endl;
      exit(0);
    }
    /** END of sending **/

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

    /** Waiting Ack **/
    bool ack_corrupted = false;

    char buf[ACK_SIZE];
    do {
      socklen_t addrlen = sizeof(rem_addr);
      bytes_snd_rcv = recvfrom(udp_socketfd, buf, ACK_SIZE, 0,
                              (struct sockaddr *)&rem_addr, &addrlen);
      if (bytes_snd_rcv > 1)
        buf[bytes_snd_rcv] = 0; // set last byte 0

      memcpy(&ack, buf, ACK_SIZE);
      if(ack.len != 8 || ack.ackno != pkt.seqno) { // corrupted
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
      }
    } while(ack_corrupted && !stop);
  }
}

void stop_and_wait::receive(char* data) {
  // TODO
  socklen_t addrlen = sizeof(rem_addr);
  bytes_snd_rcv = recvfrom(udp_socketfd, data, PACKET_SIZE, 0,
                          (struct sockaddr *)&rem_addr, &addrlen);

  if (bytes_snd_rcv > 1)
    data[bytes_snd_rcv] = 0; // set last byte 0

  memcpy(&pkt, data, PACKET_SIZE);

  if (pkt.len <= PACKET_SIZE) {
    //checksum of pkt sent
    //=======
    //if (chksum == pkt.chksum) {
    //	send_ack(pkt.seqno);
    //}
    cout << pkt.seqno << endl;
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
  ack.ackno = ackno;
  //checksum of ack which will be sent
  //==========
  //ack.chksum = chksum;

  char data[ACK_SIZE];
  memcpy(data, &ack, ACK_SIZE);

  bytes_snd_rcv = sendto(udp_socketfd, data, ACK_SIZE, 0,
                        (struct sockaddr *)&rem_addr, sizeof(rem_addr));
  if(bytes_snd_rcv == -1)
    cout <<"stop_and_wait::send_ack : ERROR sending ack"<<endl;
}

void stop_and_wait::on_timeout(int alarm_id) {
 cout << "TIMEOUT!" << endl;
  // retransmet packet
  if(alarm_id == pkt.seqno){
    char data[PACKET_SIZE];
    memcpy(data, &pkt, PACKET_SIZE);
    Alarm alarm;
    alarm.set_alarm_listner(this);
    alarm.start(time_out, pkt.seqno);
    cout<<"sending packet seqno:"<<pkt.seqno<<endl;
    int bytes_snd_rcv = sendto(udp_socketfd, data, PACKET_SIZE, 0,
                        (struct sockaddr *)&rem_addr, sizeof(rem_addr));
    if(bytes_snd_rcv == -1) {
      cout << "Error in sending packet!" << endl;
      exit(0);
    }
  }
}
stop_and_wait::~stop_and_wait() {
  r_close(); // TODO(houssainy) what if it's already stopped!
}
