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
    bool success = read_all(connection.fd, message);
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
    if (is_writeable(connection.fd, TCP_TIMEOUT_SEC, TCP_TIMEOUT_USEC)) {
        write(connection.fd, response->get_raw());
    }
    connection.terminate();
    decrease_connection();
}


int webserver::read(int socket, char *ptr, size_t size) {
    if (is_readable(socket, TCP_TIMEOUT_SEC, TCP_TIMEOUT_USEC)) {
        return recv(socket, ptr, TCP_BUFFER_SIZE, 0);
    }
    return -1;
}


bool webserver::read_all(int socket, std::string &data) {
    const int MAX_BUFFER = 65535;
    char buf[MAX_BUFFER];

    if (is_readable(socket, TCP_TIMEOUT_SEC, TCP_TIMEOUT_USEC)) {
        int n = read(socket, buf, MAX_BUFFER);
        if (n > 0) {
            buf[n] = '\n';
            data = std::string(buf);
            return true;
        }
    }
    return false;
}

void webserver::write(int socket, const std::string &message) {
    ssize_t bytes_sent = send(socket, message.c_str(), message.length(), 0);
    if (bytes_sent == -1) {
        perror("can't send data to client");
        throw std::runtime_error("can't send data to client");
    }
}

void webserver::set_port(int port) {
    this->port = port;
}

void webserver::set_max_connection(int max_connection) {
    this->max_connection = max_connection;
}

bool webserver::is_readable(int socket, int timeout_sec, int timeout_usec) {
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(socket, &rfds);
    timeval tv;
    tv.tv_usec = timeout_usec;
    tv.tv_sec = timeout_sec;
    ssize_t v = select(static_cast<int>(socket + 1), &rfds, nullptr, nullptr, &tv);
    return  v > 0;
}

bool webserver::is_writeable(int socket, int timeout_sec, int timeout_usec) {
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
