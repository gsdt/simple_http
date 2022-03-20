//
// Created by tuan on 19/03/2022.
//

#include "http_request.hpp"

http_request::http_request() {

}

http_request::http_request(const std::string &raw_message) {
    auto split_result = split(raw_message, CRLF + CRLF, 2);
    if (split_result.size() == 2) {
        this->body = split_result[1];
    }
    if (split_result.size() >= 1) {
        this->parse_header(split_result[0]);
    }
}

std::string http_request::get_request_uri() {
    return this->request_uri;
}

void http_request::parse_header(const std::string &raw_header) {
    auto lines = split(raw_header, CRLF);
    if (lines.size() >= 1) {
        auto cols = split(lines[0], " ");
        if (cols.size() >= 3) {
            this->version = trim(cols[2]);
        }
        if (cols.size() >= 2) {
            this->request_uri = trim(cols[1]);
        }
        if (cols.size() >= 1) {
            this->method = trim(cols[0]);
        }
    }

    for (int i = 1; i < lines.size(); i++) {
        auto cols = split(lines[i], ":", 2);
        if (cols.size() == 2) {
            this->headers.emplace(trim(cols[0]), trim(cols[1]));
        }
    }

}

std::string http_request::get_method() {
    return this->method;
}

std::string http_request::get_version() {
    return this->version;
}

std::string http_request::get_header(const std::string &header_key) {
    auto it = this->headers.find(header_key);
    if (it != this->headers.end()) {
        return it->second;
    }
    return "";
}

std::string http_request::get_body() {
    return this->body;
}

void http_request::set_endpoint(const http_endpoint &ep) {
    this->endpoint = ep;
}

http_endpoint http_request::get_endpoint() {
    return this->endpoint;
}

