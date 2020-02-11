#include <iostream>
#include <tcp_server/tcp_server.h>

int main() {
    tcp_server *s = new tcp_server(1114);
    s->start();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
