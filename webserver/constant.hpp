//
// Created by tuan on 19/03/2022.
//

#ifndef TUAN_HTTP_CONSTANT_HPP
#define TUAN_HTTP_CONSTANT_HPP

#include <string>
#include <unordered_map>

const int TCP_BUFFER_SIZE = 1024;
const int TCP_TIMEOUT_SEC = 5;
const int TCP_TIMEOUT_USEC = 0;

const int TCP_TIMEOUT_ERROR = -1;
const int TCP_UNEXPECTED_CLOSE_ERROR = -2;
const int TCP_UNKNOWN_ERROR = -3;
const int TCP_RECV_ERROR = -4;

const std::string GET_METHOD = "GET";
const std::string POST_METHOD = "POST";
const std::string PUT_METHOD = "PUT";
const std::string DELETE_METHOD = "DELETE";
const std::string OPTIONS_METHOD = "OPTIONS";

const std::string CRLF = "\r\n";
const std::string WHITESPACE = " \n\r\t\f\v";
const std::string HTTP1_1 = "HTTP/1.1";

const std::unordered_map<int, std::string> STATUS_LINES = {
        {200, "HTTP/1.1 200 OK"},
        {201, "HTTP/1.1 201 Created"},
        {202, "HTTP/1.1 202 Accepted"},
        {204, "HTTP/1.1 204 No Content"},

        {300, "HTTP/1.1 300 Multiple Choices"},
        {301, "HTTP/1.1 301 Moved Permanently"},
        {302, "HTTP/1.1 302 Moved Temporarily"},
        {304, "HTTP/1.1 304 Not Modified"},

        {400, "HTTP/1.1 400 Bad Request"},
        {401, "HTTP/1.1 401 Unauthorized"},
        {403, "HTTP/1.1 403 Forbidden"},
        {404, "HTTP/1.1 404 Not Found"},
        {413, "HTTP/1.1 413 Payload Too Large"},
        {422, "HTTP/1.1 422 Unprocessable Entity"},
        {429, "HTTP/1.1 429 Too Many Requests"},

        {500, "HTTP/1.1 500 Internal Server Error"},
        {501, "HTTP/1.1 501 Not Implemented"},
        {502, "HTTP/1.1 502 Bad Gateway"},
        {503, "HTTP/1.1 503 Service Unavailable"},
};

const std::unordered_map<std::string, std::string> FILE_CONTENT_TYPE = {
        {"css",  "text/css"},
        {"html", "text/html"},
        {"js",   "text/javascript"},
        {"jpg",  "image/jpeg"},
        {"jpeg", "image/jpeg"},
        {"png",  "image/png"},
        {"ico",  "image/x-icon"},
};

#endif //TUAN_HTTP_CONSTANT_HPP
