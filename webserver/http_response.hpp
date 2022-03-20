//
// Created by tuan on 19/03/2022.
//

#ifndef TUAN_HTTP_HTTP_RESPONSE_HPP
#define TUAN_HTTP_HTTP_RESPONSE_HPP

#include <string>
#include <unordered_map>
#include "constant.hpp"
#include "string_utils.hpp"

class http_response {
private:
    int status_code;
    std::string body;
    std::unordered_map<std::string, std::string> headers;
public:
    http_response();

    std::string get_raw();

    void set_status_code(int status_code);

    void set_body(const std::string &body);

    void add_header(const std::string &key, const std::string &value);

    void set_content_type(const std::string &content_type);
};


#endif //TUAN_HTTP_HTTP_RESPONSE_HPP
