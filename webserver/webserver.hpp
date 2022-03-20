//
// Created by tuan on 19/03/2022.
//

#ifndef TUAN_HTTP_WEBSERVER_HPP
#define TUAN_HTTP_WEBSERVER_HPP

#ifndef THREAD_POOL_COUNT
#define THREAD_POOL_COUNT                                           \
  ((std::max)(8u, std::thread::hardware_concurrency() > 0           \
                      ? std::thread::hardware_concurrency() - 1     \
                      : 0))
#endif

#include <unordered_map>

#include "http_resource.hpp"
#include "http_endpoint.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "tcp_server.hpp"
#include "thread_pool.hpp"

#include <iostream>
#include <stdexcept>

template <typename T> inline ssize_t handle_EINTR(T fn) {
    ssize_t res = false;
    while (true) {
        res = fn();
        if (res < 0 && errno == EINTR) { continue; }
        break;
    }
    return res;
}

class webserver {
private:
    int port;

    int connection_count;

    int max_connection;

    std::mutex mt;

    std::vector<http_endpoint> registered_resources;

    std::unordered_map<http_endpoint, http_resource *> resource_map;

    void process_socket(const tcp_connection &connection);

    int read(int socket, char* ptr, size_t size);

    bool read_all(int socket, std::string &data);

    void write(int socket, const std::string &message);

    bool is_readable(int socket, int timeout_sec, int timeout_usec);

    bool is_writeable(int socket, int timeout_sec, int timeout_usec);

    void increase_connection();

    void decrease_connection();
public:
    explicit webserver();

    void set_port(int port);

    void set_max_connection(int max_connection);

    void start();

    void register_resource(const std::string &path, http_resource *hr);
};


#endif //TUAN_HTTP_WEBSERVER_HPP
