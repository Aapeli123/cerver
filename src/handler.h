#ifndef HANDLER_H
#define HANDLER_H
#include <stdio.h>

#include "response.h"
#include "http.h"

int handle_request(char* req_buffer, int req_size, int client_fd);
#endif