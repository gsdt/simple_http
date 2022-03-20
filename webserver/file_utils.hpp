//
// Created by tuan on 20/03/2022.
//

#ifndef TUAN_HTTP_FILE_UTILS_HPP
#define TUAN_HTTP_FILE_UTILS_HPP

#include <vector>
#include <fstream>
#include <sys/stat.h>

inline std::vector<char> read_binary_file(const std::string &filepath) {
    std::ifstream input(filepath, std::ios::binary);
    std::vector<char> bytes(
            (std::istreambuf_iterator<char>(input)),
            (std::istreambuf_iterator<char>()));
    input.close();
    return bytes;
}

inline bool is_file_exist(const std::string &name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

#endif //TUAN_HTTP_FILE_UTILS_HPP
