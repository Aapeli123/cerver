#ifndef SERVER_H
#define SERVER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
// system file contains definations of system calls
// include<sys/socket.h>
#include <winsock.h>
//#inlude<netinet/in.h>
#include <ws2tcpip.h>
#include <ws2ipdef.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

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