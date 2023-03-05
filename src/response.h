#ifndef RESPONSE_H
#define RESPONSE_H

#include <sys/socket.h>

int write_response(int client_fd, char* response, int response_bytes);

#endif