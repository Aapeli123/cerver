#include "handler.h"

static char* resolve_path(char* path) {
    char* content = (char*)hashmap_get(config->route_map, path);
    if(content!=NULL) return content;

    char* path_wildcard = calloc(1, strlen(path) + 2);
    strcat(path_wildcard, path);
    strcat(path_wildcard, "*");

    content = (char*)hashmap_get(config->route_map, path_wildcard);
    free(path_wildcard);
    if(content!=NULL) return content;

    // TODO Resolve double wildcard path

    return config->fallback_page;
}


int handle_request(char *req_buffer, int req_size, int client_fd)
{
//    struct http_resp res = {.content = "Welcome to cerver", .header_count = 2, .headers = headers, .http_ver = "HTTP/1.1", .status = 200, .reason_str = "OK"};
    // char *resp = http_stringify_resp(&res);
    if (req_buffer == NULL)
    {
        // write_response(client_fd, resp, strlen(resp) + 1);
        // free(resp);

        return 0;
    }
    struct http_req *req = (struct http_req *)calloc(1, sizeof(struct http_req));
    int r = http_parse_req(req_buffer, req_size, req);
    if (r < 0)
    {
        http_req_free(req);
        // free(resp);
        return -1;
    }
    char* content = resolve_path(req->path);
    char* mime = mime_for_filename(req->path);
    int content_len = strlen(content);
    char content_len_char[128];
    sprintf(content_len_char, "%d", content_len);
    struct http_header headers[] = {{.key = "Content-Type", .value = mime}, {.key = "Content-Length", .value = content_len_char}};
    char* resp = http_response_200(content, headers, 2);
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
        if(bufSize >= 1024 * 1000) { // Max req size is 1000 KB
            shutdown(fd, SHUT_RDWR);
            free(buffer);
            printf("ERR: Req from fd %d was too long. Aborting...\n", fd);
            return;
        }
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
    } while(n == BUFFER_SIZE);
    

    handle_request(buffer, bufSize, fd);
    shutdown(fd, SHUT_RDWR);
    close(fd);
    free(buffer);
    // free(fd);
}

