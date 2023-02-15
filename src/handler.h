#ifndef HANDLER_H
#define HANDLER_H
#include "response.h"
#include <stdio.h>

int handle_request(char* req_buffer, int req_size, int client_fd);
#endif