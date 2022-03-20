//
// Created by tuan on 20/03/2022.
//

#ifndef TUAN_HTTP_TCP_CONNECTION_HPP
#define TUAN_HTTP_TCP_CONNECTION_HPP

#include <netinet/in.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include "constant.hpp"

class tcp_connection {
private:
    int socket;
    struct sockaddr_in address;
public:
    tcp_connection(int socket, struct sockaddr_in address) {
        this->socket = socket;
        this->address = address;
    }

    void terminate() const {
        close(socket);
    }

    int read(char *ptr, size_t size) const {
        if (is_readable(TCP_TIMEOUT_SEC, TCP_TIMEOUT_USEC)) {
            return recv(socket, ptr, TCP_BUFFER_SIZE, 0);
        }
        return -1;
    }

    bool read_all(std::string &data) const {
        const int MAX_BUFFER = 65535;
        char buf[MAX_BUFFER];

        if (is_readable(TCP_TIMEOUT_SEC, TCP_TIMEOUT_USEC)) {
            int n = read(buf, MAX_BUFFER);
            if (n > 0) {
                buf[n] = '\n';
                data = std::string(buf);
                return true;
            }
        }
        return false;
    }

    void write(const std::string &message) const {
        ssize_t bytes_sent = send(socket, message.c_str(), message.length(), 0);
        if (bytes_sent == -1) {
            perror("can't send data to client");
            throw std::runtime_error("can't send data to client");
        }
    }

    bool is_readable(int timeout_sec, int timeout_usec) const {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(socket, &rfds);
        timeval tv;
        tv.tv_usec = timeout_usec;
        tv.tv_sec = timeout_sec;
        ssize_t v = select(static_cast<int>(socket + 1), &rfds, nullptr, nullptr, &tv);
        return v > 0;
    }

    bool is_writeable(int timeout_sec, int timeout_usec) const {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(socket, &fds);

        timeval tv;
        tv.tv_sec = timeout_sec;
        tv.tv_usec = timeout_usec;

        int val = select(socket + 1, nullptr, &fds, nullptr, &tv);
        if (val == 0) {
            perror("select timeout");
        } else if (val == -1) {
            perror("select error");
        }
        return val > 0;
    }

    ssize_t get_address_size() {
        return sizeof(address);
    }

};

#endif //TUAN_HTTP_TCP_CONNECTION_HPP
