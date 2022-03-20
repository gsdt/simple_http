//
// Created by tuan on 19/03/2022.
//

#include "webserver.hpp"

webserver::webserver() {
    std::cout << "[#] total thread pool count: " << THREAD_POOL_COUNT << std::endl;
    this->port = 8080;
    this->connection_count = 0;
    this->max_connection = 10000;
}

void webserver::start() {
    tcp_server tcp(this->port);
    tcp.setup();

    ThreadPool pool(THREAD_POOL_COUNT);
    while (true) {
        while (this->connection_count >= this->max_connection) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        tcp_connection connection = tcp.accept_client();
        pool.enqueue([this, connection] {
            process_socket(connection);
        });
    }
}

void webserver::register_resource(const std::string &path, http_resource *hr) {
    http_endpoint endpoint(path);
    registered_resources.push_back(endpoint);
    resource_map.emplace(endpoint, hr);
}

void webserver::process_socket(const tcp_connection &connection) {
    this->increase_connection();
    std::string message;
    bool success = connection.read_all(message);
    if (!success) {
        connection.terminate();
        decrease_connection();
        return;
    }
    std::shared_ptr<http_request> request(new http_request(message));
    std::shared_ptr<http_response> response(new http_response());

    if (request->get_version() != HTTP1_1) {
        response->set_status_code(400);
        response->set_body("illegal request");
    } else {
        bool is_url_match = false;
        for (const auto &endpoint: this->registered_resources) {
            http_resource *resource = this->resource_map.at(endpoint);

            request->set_endpoint(endpoint);

            if (endpoint.match(request->get_request_uri())) {
                is_url_match = true;
                try {
                    resource->handle(request, response);
                } catch (...) {
                    response->set_status_code(500);
                    response->set_body("Internal sever error");
                }
                break;
            }
        }

        if (!is_url_match) {
            response->set_status_code(404);
            response->set_body("Not found");
        }
    }
    if (connection.is_writeable(TCP_TIMEOUT_SEC, TCP_TIMEOUT_USEC)) {
        connection.write(response->get_raw());
    }
    connection.terminate();
    decrease_connection();
}

void webserver::set_port(int bind_port) {
    this->port = bind_port;
}

void webserver::set_max_connection(int no_connection) {
    this->max_connection = no_connection;
}

void webserver::increase_connection() {
    std::lock_guard<std::mutex> lock(mt);
    this->connection_count++;
    std::cout << "[+] total handling connection: " << this->connection_count << std::endl;

}

void webserver::decrease_connection() {
    std::lock_guard<std::mutex> lock(mt);
    this->connection_count--;
    std::cout << "[-] total handling connection: " << this->connection_count << std::endl;
}
