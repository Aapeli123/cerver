#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HEADER_BUF_SIZE 2048


struct http_header;
struct http_req;
struct http_resp;

void header_buf_free(struct http_header *headers, int n);
void http_req_free(struct http_req *req);
int http_parse_req(char *http_req, int len, struct http_req *req);
#endif