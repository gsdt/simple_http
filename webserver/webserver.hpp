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

class webserver {
private:
    int port;

    int connection_count;

    int max_connection;

    std::mutex mt;

    std::vector<http_endpoint> registered_resources;

    std::unordered_map<http_endpoint, http_resource *> resource_map;

    void process_socket(const tcp_connection &connection);

    void increase_connection();

    void decrease_connection();

public:
    explicit webserver();

    void set_port(int bind_port);

    void set_max_connection(int no_connection);

    void start();

    void register_resource(const std::string &path, http_resource *hr);
};


#endif //TUAN_HTTP_WEBSERVER_HPP
