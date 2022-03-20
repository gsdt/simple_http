//
// Created by tuan on 19/03/2022.
//

#ifndef TUAN_HTTP_TCP_SERVER_HPP
#define TUAN_HTTP_TCP_SERVER_HPP

#include "tcp_connection.hpp"

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
