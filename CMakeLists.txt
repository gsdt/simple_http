cmake_minimum_required(VERSION 3.17)
project(tuan_http)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

add_executable(tuan_http main.cpp webserver/webserver.cpp webserver/webserver.hpp webserver/http_resource.cpp webserver/http_resource.hpp webserver/http_endpoint.cpp webserver/http_endpoint.hpp webserver/http_request.cpp webserver/http_request.hpp webserver/constant.hpp webserver/http_response.cpp webserver/http_response.hpp webserver/string_utils.hpp webserver/tcp_server.cpp webserver/tcp_server.hpp webserver/thread_pool.hpp webserver/file_resource.cpp webserver/file_resource.hpp webserver/file_utils.hpp)