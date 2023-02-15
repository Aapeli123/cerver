#include "handler.h"

int handle_request(char* req_buffer, int req_size, int client_fd) {
    struct http_req* req = (struct http_req*)calloc(1, sizeof(struct http_req));
    http_parse_req(req_buffer, req_size, req);
    write_response(client_fd, req_buffer, req_size);
    return 0;
}