//
// Created by tuan on 20/03/2022.
//

#include "file_resource.hpp"


void file_resource::handle(
        const std::shared_ptr<http_request> &request,
        const std::shared_ptr<http_response> &response) {
    std::string file_path =
            root_path + "/" + request->get_endpoint().get_unmatch_path(request->get_request_uri());

    std::vector<char> file_data;
    bool file_exist = true;
    if (this->cache.count(file_path) == 0) {
        std::cout << " - requesting file:" << file_path << std::endl;
        if (is_file_exist(file_path)) {
            file_data = read_binary_file(file_path);
            this->cache.emplace(file_path, file_data);
        } else {
            file_exist = false;
            this->cache.emplace(file_path, std::vector<char>());
        }
    } else {
        file_data = this->cache.at(file_path);
        file_exist = file_data.size() > 0;
    }

    if (!file_exist) {
        response->set_status_code(404);
        response->set_body("file not found");
        return;
    }

    response->set_status_code(200);
    response->set_body(std::string(file_data.begin(), file_data.end()));
    std::string file_ext = file_extension(file_path);
    std::string file_content_type = get_file_content_type(file_ext);
    response->set_content_type(file_content_type);
}

std::string file_resource::get_file_content_type(const std::string &ext) {
    if (FILE_CONTENT_TYPE.count(ext) == 1) {
        return FILE_CONTENT_TYPE.at(ext);
    }
    return "text/plaintext";
}

file_resource::file_resource(const std::string &path)
        : root_path(std::move(path)) {
    std::cout << " - root path: " << path << std::endl;
}
