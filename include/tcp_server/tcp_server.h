//
// Created by localcc on 2/4/20.
//

#ifndef PASSXTENDED_SERVER_TCP_SERVER_H
#define PASSXTENDED_SERVER_TCP_SERVER_H
class tcp_server {
public:
    tcp_server(int port);
    ~tcp_server();
    void start();
    void stop();
private:
    int sock_fd;
    bool running;
};


#endif //PASSXTENDED_SERVER_TCP_SERVER_H
