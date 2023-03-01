#ifndef HANDLER_H
#define HANDLER_H
#include <stdio.h>
#include <unistd.h>

#include "response.h"
#include "http.h"

#define BUFFER_SIZE 5


int handle_request(char* req_buffer, int req_size, int client_fd);
void handler_worker(void* client_fd);
#endif