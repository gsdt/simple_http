//
// Created by tuan on 19/03/2022.
//

#ifndef TUAN_HTTP_STRING_UTILS_HPP
#define TUAN_HTTP_STRING_UTILS_HPP

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <regex>
#include "constant.hpp"

static inline std::string ltrim(const std::string &s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

static inline std::string rtrim(const std::string &s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

static inline std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}


static inline std::vector<std::string>
split(const std::string &str, const std::string &delimiter = " ", const int max_elements = 0) {
    std::vector<std::string> tokens;
    std::string::size_type start_index = 0;
    while (true) {
        std::string::size_type next_index = str.find(delimiter, start_index);
        if (next_index == std::string::npos) {
            tokens.push_back(str.substr(start_index));
            break;
        } else {
            tokens.push_back(str.substr(start_index, next_index - start_index));
            start_index = next_index + delimiter.length();
        }
        if (max_elements > 0 && tokens.size() == max_elements - 1) {
            tokens.push_back(str.substr(start_index));
            break;
        }
    }

    return tokens;
}

static inline void msg_add_line(std::string &data, const std::string &new_line) {
    data.append(new_line);
    data.append(CRLF);
}

static inline void msg_add_header(std::string &data, const std::string &key, const std::string &value) {
    data.append(key);
    data.append(": ");
    data.append(value);
    data.append(CRLF);
}

static inline void msg_add_body(std::string &data, const std::string &body) {
    data.append(CRLF);
    data.append(body);
}

static inline std::string get_status_line(int status_code) {
    if (STATUS_LINES.find(status_code) == STATUS_LINES.end()) {
        return STATUS_LINES.at(200);
    }
    return STATUS_LINES.at(status_code);
}

inline std::string file_extension(const std::string &path) {
    std::smatch m;
    static auto re = std::regex("\\.([a-zA-Z0-9]+)$");
    if (std::regex_search(path, m, re)) { return m[1].str(); }
    return std::string();
}

#endif //TUAN_HTTP_STRING_UTILS_HPP
