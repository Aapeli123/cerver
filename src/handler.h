#ifndef HANDLER_H
#define HANDLER_H
#include <stdio.h>

#include "response.h"
#include "http.h"

#define BUFFER_SIZE 2048


int handle_request(char* req_buffer, int req_size, int client_fd);
void handler_worker(void* client_fd);
#endif