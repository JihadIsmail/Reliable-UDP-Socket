#ifndef SERVER_SIMULATOR_H
#define SERVER_SIMULATOR_H


class server_simulator {
  private:
    R_UDP *socket;
  public:
    server_simulator();

    void receive_requested_file_path(char *file_path);
    void send_file(char *file_path);

    virtual ~server_simulator();
};

#endif // SERVER_SIMULATOR_H
