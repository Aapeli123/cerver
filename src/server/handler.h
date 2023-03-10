#ifndef HANDLER_H
#define HANDLER_H
#include <stdio.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <signal.h>

#include "response.h"
#include "../http/http.h"
#include "../config/config_t.h"
#include "../mime/mime.h"


#define BUFFER_SIZE 1024

extern config_t* config;

int handle_request(char* req_buffer, int req_size, int client_fd, SSL* ssl);
void handler_worker(void* client_fd);
void handler_ssl_worker(void* client_fd);
#endif