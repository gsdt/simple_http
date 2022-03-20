//
// Created by tuan on 19/03/2022.
//

#ifndef TUAN_HTTP_TCP_SERVER_HPP
#define TUAN_HTTP_TCP_SERVER_HPP

#include <netinet/in.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>


struct tcp_connection {
    int fd;
    struct sockaddr_in address;

    void terminate() const {
        close(fd);
    }
};

class tcp_server {
private:
    int server_fd;
    int port;
public:
    tcp_server();
    explicit tcp_server(int port);
    void setup();
    tcp_connection accept_client();
};


#endif //TUAN_HTTP_TCP_SERVER_HPP
