//
// Created by tuan on 20/03/2022.
//

#ifndef TUAN_HTTP_FILE_RESOURCE_HPP
#define TUAN_HTTP_FILE_RESOURCE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "http_resource.hpp"
#include "file_utils.hpp"
#include "constant.hpp"

class file_resource : public http_resource {
private:
    const std::string root_path;
    std::unordered_map<std::string, std::vector<char>> cache;

    std::string get_file_content_type(const std::string &ext);

public:
    explicit file_resource(const std::string &path);

    void handle(const std::shared_ptr<http_request> &request, const std::shared_ptr<http_response> &response) override;
};


#endif //TUAN_HTTP_FILE_RESOURCE_HPP
