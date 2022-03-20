
#include "webserver/webserver.hpp"
#include "webserver/file_resource.hpp"
#include <iostream>

using namespace std;

class simple_resource : public http_resource {
    void handle(const std::shared_ptr<http_request> &request, const std::shared_ptr<http_response> &response) {
//        std::cout << request->get_version() << std::endl;
//        std::cout << request->get_method() << std::endl;
//        std::cout << request->get_request_uri() << std::endl;

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
