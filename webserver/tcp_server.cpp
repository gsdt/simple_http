//
// Created by tuan on 19/03/2022.
//

#include "tcp_server.hpp"

tcp_server::tcp_server(int port): port(port) {
}

void tcp_server::setup() {

    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    timeval tv;
    tv.tv_sec = static_cast<long>(10);
    tv.tv_usec = static_cast<decltype(tv.tv_usec)>(0);

    int opt = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv))) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv))) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 32) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

tcp_connection tcp_server::accept_client() {
    tcp_connection connection{};
    socklen_t addrlen = sizeof(connection.address);
    if ((connection.fd = accept(server_fd, (struct sockaddr *) &connection.address, &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return connection;
}

tcp_server::tcp_server(): port(8080) {

}
