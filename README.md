# simple HTTP server

## compile
```bash
cmake .
make clean && make
```

## run
```bash
./bin/http_server
```

## test
open browser and access 

[http://localhost:8080/test](http://localhost:8080/test)

[http://localhost:8080/index.html](http://localhost:8080/index.html)

## feature list

### 1. general feature
    
   - support thread pool to handle concurrent connection
    
### 2. http_request
```cpp
public:
    http_request();

    explicit http_request(const std::string &raw_message);

    std::string get_request_uri();

    std::string get_method();

    std::string get_version();

    std::string get_body();

    void set_endpoint(const http_endpoint &ep);

    http_endpoint get_endpoint();

    std::string get_header(const std::string &header_key);

    void parse_header(const std::string &raw_header);
};
```
    
### 3. http_response

```cpp
public:
    http_response();

    # get raw http response to send to client
    std::string get_raw();

    void set_status_code(int status_code);

    void set_body(const std::string &body);

    void add_header(const std::string &key, const std::string &value);

    void set_content_type(const std::string &content_type);
};
```
 
### 4. webserver

```cpp
public:
    explicit webserver();

    void set_port(int port);

    void set_max_connection(int max_connection);

    void start();

    void register_resource(const std::string &path, http_resource *hr);
```

feature
- can handle server error (but can not custom error page)
- can handle file not found error (but can not custom error page)
- default timeout when read data from client is 5s

example
```cpp
#include "webserver/webserver.hpp"
#include "webserver/file_resource.hpp"
#include <iostream>

class simple_resource : public http_resource {
    void handle(const std::shared_ptr<http_request> &request, const std::shared_ptr<http_response> &response) {
        response->set_status_code(200);
        response->set_content_type("application/json");
        response->set_body("{\"status\":\"success\"}");
    }
};

int main() {
    webserver server;
    server.set_port(8080);
    server.set_max_connection(12000);

    simple_resource simple;
    file_resource file_handler("html");
    server.register_resource("/test", &simple);
    server.register_resource("/", &file_handler);
    server.start();
}

```
    
### 5. http_resource

```cpp
class http_resource {
public:
    http_resource();
    virtual void handle(const std::shared_ptr<http_request>& request, const std::shared_ptr<http_response>& response) = 0;
};
```
    
### 6. file resource
- use to host a folder
- supported file extension: html, css, js, jpeg, png
- other file extension will consider as plaintext


## future plan
- support cookies
- support multipart data
- support client can send large data (more than 65535 bytes)