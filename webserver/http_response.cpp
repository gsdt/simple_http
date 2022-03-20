//
// Created by tuan on 19/03/2022.
//

#include "http_response.hpp"


http_response::http_response() {

}

std::string http_response::get_raw() {
    std::string data;

    msg_add_line(data, get_status_line(this->status_code));
    for (const auto &it: this->headers) {
        msg_add_header(data, it.first, it.second);
    }

    if (this->headers.count("Content-Length") == 0) {
        msg_add_header(data, "Content-Length", std::to_string(this->body.length()));
    }

    if (this->headers.count("Content-Type") == 0) {
        msg_add_header(data, "Content-Type", "text/html");
    }

    msg_add_body(data, body);
    return data;
}

void http_response::set_status_code(int status_code) {
    this->status_code = status_code;
}

void http_response::set_body(const std::string &body) {
    this->body = body;
}

void http_response::add_header(const std::string &key, const std::string &value) {
    this->headers.emplace(key, value);
}

void http_response::set_content_type(const std::string &content_type) {
    this->headers.emplace("Content-Type", content_type);
}

