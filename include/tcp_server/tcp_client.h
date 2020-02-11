//
// Created by localcc on 2/4/20.
//

#ifndef PASSXTENDED_SERVER_TCP_CLIENT_H
#define PASSXTENDED_SERVER_TCP_CLIENT_H
class tcp_client {
public:
    tcp_client(int client_sockfd);
    ~tcp_client();
    void handle();
private:
    int sock_fd;
};
#endif //PASSXTENDED_SERVER_TCP_CLIENT_H
