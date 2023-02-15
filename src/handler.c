#include "handler.h"

int handle_request(char* req_buffer, int req_size, int client_fd) {
    struct http_req* req = (struct http_req*)calloc(1, sizeof(struct http_req));
    http_parse_req(req_buffer, req_size, req);

    struct http_header headers[] = {{.key = "Host", .value = "localhost:8080"}, {.key = "Content-Type", .value = "text/plain"}, {.key = "Content-Length", .value = "10"}};
    struct http_resp res = {.content = "HelloWOlrd", .header_count = 3, .headers = headers, .http_ver="HTTP/1.1", .status=200, .reason_str="OK"};
    char* resp = http_stringify_resp(&res);

    write_response(client_fd, resp, strlen(resp) + 1);
    free(resp);

    return 0;
}