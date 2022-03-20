//
// Created by tuan on 19/03/2022.
//

#ifndef TUAN_HTTP_HTTP_REQUEST_HPP
#define TUAN_HTTP_HTTP_REQUEST_HPP

#include "constant.hpp"
#include "string_utils.hpp"
#include "http_endpoint.hpp"

#include <string>
#include <unordered_map>

class http_request {
private:
    std::string method;
    std::string request_uri;
    std::string version;
    std::unordered_multimap<std::string, std::string> headers;
    std::string body;
    http_endpoint endpoint;

    void parse_header(const std::string &raw_header);

public:
    http_request();

    explicit http_request(const std::string &raw_message);

    std::string get_request_uri();

    std::string get_method();

    std::string get_version();

    std::string get_body();

    void set_endpoint(const http_endpoint &ep);

    http_endpoint get_endpoint();

    std::string get_header(const std::string &header_key);

};


#endif //TUAN_HTTP_HTTP_REQUEST_HPP
