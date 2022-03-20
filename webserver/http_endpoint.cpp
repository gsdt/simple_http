//
// Created by tuan on 19/03/2022.
//

#include "http_endpoint.hpp"

#include <utility>

http_endpoint::http_endpoint(std::string url) :
        complete_url(std::move(url)) {
}

bool http_endpoint::match(const std::string &url) const {
    if (this->complete_url.size() > url.size()) {
        return false;
    }
    if (this->complete_url == url) {
        return true;
    }
    if (url.find(this->complete_url) == 0) {
        if (this->complete_url.back() == '/') {
            char c = url[this->complete_url.length()-1];
            return c == '/';
        }
        char c = url[this->complete_url.length()];
        return c == '/' || c == '\0';
    }
    return false;
}

bool http_endpoint::operator==(const http_endpoint &h) const {
    return this->complete_url == h.complete_url;
}

std::string http_endpoint::get_unmatch_path(const std::string &path) {
    return path.substr(this->complete_url.size());
}

http_endpoint::http_endpoint() {

}
