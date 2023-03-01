#include "handler.h"

int handle_request(char *req_buffer, int req_size, int client_fd)
{
    struct http_header headers[] = {{.key = "Content-Type", .value = "text/plain"}, {.key = "Content-Length", .value = "17"}};
    struct http_resp res = {.content = "Welcome to cerver", .header_count = 2, .headers = headers, .http_ver = "HTTP/1.1", .status = 200, .reason_str = "OK"};
    char *resp = http_stringify_resp(&res);
    if (req_buffer == NULL)
    {
        write_response(client_fd, resp, strlen(resp) + 1);
        free(resp);

        return 0;
    }
    struct http_req *req = (struct http_req *)calloc(1, sizeof(struct http_req));
    int r = http_parse_req(req_buffer, req_size, req);
    if (r < 0)
    {
        http_req_free(req);
        free(resp);
        return -1;
    }
    write_response(client_fd, resp, strlen(resp) + 1);
    free(resp);
    http_req_free(req);
    return 0;
}

void handler_worker(void *client_fd)
{
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
    int fd = (int)client_fd; // It's an int, source: "Trust me bro"
    char *buffer = (char *)calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        return;
    }
    int n = 0;
    int i = 0;
    int bufSize = BUFFER_SIZE;
    do {
        buffer = realloc(buffer, bufSize);
        n = read(fd, buffer + BUFFER_SIZE * sizeof(char) * i, BUFFER_SIZE);
        if (n == -1)
        {
            perror("ERR");
            shutdown(fd, SHUT_RDWR);
            free(buffer);
            return;
        }
        bufSize += BUFFER_SIZE;
        i++;
        // TODO extend buffer if size is too small
    } while(n == BUFFER_SIZE);
    

    handle_request(buffer, bufSize, fd);
    shutdown(fd, SHUT_RDWR);
    close(fd);
    free(buffer);
    // free(fd);
}