//
// Created by localcc on 2/4/20.
//
#include <tcp_server/tcp_server.h>
#include <tcp_server/tcp_client.h>
#include <tls_helper/tls_helper.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <thread>
#include <cstdio>
#include <cstring>
#include <unistd.h>

tcp_server::tcp_server(int port) {
    this->running = false;
    signal(SIGPIPE, SIG_IGN);
    this->sock_fd = socket(AF_INET, SOCK_STREAM, NULL);
    if(this->sock_fd < 0) {
        //TODO: error output
        printf("Failed to init socket\n");
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_port = htons(port);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(this->sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        //TODO: error output
        printf("Failed to bind to port\n");
    }
    tls_helper::init_tls();

}

tcp_server::~tcp_server() {
    this->sock_fd = NULL;
}

void tcp_server::start() {
    if(listen(this->sock_fd, NULL) < 0) {
        //TODO: error output
        printf("Listen failed\n");
    }
    SSL_CTX* ctx = tls_helper::gen_context();
    tls_helper::config_ctx(ctx);

    this->running = true;
    while(this->running) {
        struct sockaddr_in client_addr;
        socklen_t clilen = sizeof(client_addr);
        int client_new_sockfd = accept(sock_fd, (struct sockaddr *) &client_addr, &clilen);
        if (client_new_sockfd < 0) {
            //TODO: error output
            printf("Failed to accept client\n");
        } else {
            SSL* ssl = SSL_new(ctx);
            SSL_set_fd(ssl, client_new_sockfd);

            if(SSL_accept(ssl) < 0) {
                //TODO: error output
                printf("Failed to accept!");
            }else {
                tcp_client *c = new tcp_client(ssl);
                std::thread([&c] { c->handle(); }).detach();
                
           }
        }
    }
}

void tcp_server::stop() {
//    close(this->sock_fd);
}

