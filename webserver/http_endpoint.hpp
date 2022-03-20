//
// Created by tuan on 19/03/2022.
//

#ifndef TUAN_HTTP_HTTP_ENDPOINT_HPP
#define TUAN_HTTP_HTTP_ENDPOINT_HPP

#include <string>
#include <vector>

#include "string_utils.hpp"

class http_endpoint {
private:
    std::string complete_url;

    friend class std::hash<http_endpoint>;

public:
    http_endpoint();

    explicit http_endpoint(std::string url);

    bool match(const std::string &url) const;

    bool operator==(const http_endpoint &h) const;

    std::string get_unmatch_path(const std::string &path);
};

template<>
struct std::hash<http_endpoint> {
    std::size_t operator()(http_endpoint const &s) const noexcept {
        return std::hash<std::string>{}(s.complete_url);
    }
};

#endif //TUAN_HTTP_HTTP_ENDPOINT_HPP
