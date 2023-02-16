#include "handler.h"

int handle_request(char *req_buffer, int req_size, int client_fd)
{
    printf("%s", req_buffer);
    struct http_header headers[] = {{.key = "Content-Type", .value = "text/plain"}, {.key = "Content-Length", .value = "17"}};
    struct http_resp res = {.content = "Welcome to cerver", .header_count = 2, .headers = headers, .http_ver = "HTTP/1.1", .status = 200, .reason_str = "OK"};
    char *resp = http_stringify_resp(&res);
    if(req_buffer == NULL) {
        write_response(client_fd, resp, strlen(resp) + 1);
        free(resp);

        return 0;
    }
    struct http_req *req = (struct http_req *)calloc(1, sizeof(struct http_req));
    http_parse_req(req_buffer, req_size, req);



    write_response(client_fd, resp, strlen(resp) + 1);
    free(resp);

    return 0;
}

void handler_worker(void* client_fd) {
    int fd = (int)client_fd;
    char* buffer = (char *)calloc(1, BUFFER_SIZE);
    if(buffer == NULL) {
        return;
    }

    int n = recv(fd, buffer, BUFFER_SIZE, 0);
    printf("%d bytes recieved\n", n);
    if (n == -1)
    {
        perror("ERR");
        shutdown(fd, SHUT_RDWR);
        free(buffer);
        return;
    }
    handle_request(buffer, n, fd);
    shutdown(fd, SHUT_RDWR);
    free(buffer);
}