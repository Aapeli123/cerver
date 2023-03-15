#ifndef RESPONSE_H
#define RESPONSE_H

#ifdef WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#endif
int write_response(int client_fd, char* response, int response_bytes);

#endif