//
// Created by localcc on 2/4/20.
//

#ifndef PASSXTENDED_SERVER_TCP_CLIENT_H
#define PASSXTENDED_SERVER_TCP_CLIENT_H
#include <tls_helper/tls_helper.h>
class tcp_client {
public:
    tcp_client(SSL* ssl);
    ~tcp_client();
    void handle();
private:
    void write_int_to_sockfd(unsigned int i);
    unsigned int read_int_from_sockfd();
    SSL* ssl;
};
#endif //PASSXTENDED_SERVER_TCP_CLIENT_H
