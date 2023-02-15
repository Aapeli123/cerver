#include "handler.h"


int handle_request(char* req_buffer, int req_size, int client_fd) {
    printf("%s",req_buffer);
    write_response(client_fd, req_buffer, req_size);
    return 0;
}