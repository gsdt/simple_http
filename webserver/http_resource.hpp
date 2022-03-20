//
// Created by tuan on 19/03/2022.
//

#ifndef TUAN_HTTP_HTTP_RESOURCE_HPP
#define TUAN_HTTP_HTTP_RESOURCE_HPP

#include "http_request.hpp"
#include "http_response.hpp"

#include <memory>

class http_resource {
public:
    http_resource();
    virtual void handle(const std::shared_ptr<http_request>& request, const std::shared_ptr<http_response>& response) = 0;
};


#endif //TUAN_HTTP_HTTP_RESOURCE_HPP
