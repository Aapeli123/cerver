#ifndef SERVER_H
#define SERVER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "handler.h"
#include "../threads/thread_pool.h"
#include "../config/config_t.h"


void server_cleanup();
int server_create_socket();
int server_bind(int port);
int server_listen();
int init();
int server_start(struct thread_pool* tp);

#endif