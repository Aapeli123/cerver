#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HEADER_BUF_SIZE 2048


struct http_header
{
    char *key;
    char *value;
};

struct http_req
{
    char *path;
    char *type;
    char *http_ver;
    struct http_header *headers;
    int header_count;
};

struct http_resp
{
    char *http_ver;
    int status;
    char *reason_str;
    struct http_header *headers;
    int header_count;
    char *content;
};

void header_buf_free(struct http_header *headers, int n);
void http_req_free(struct http_req *req);
int http_parse_req(char *http_req, int len, struct http_req *req);

char* http_stringify_resp(struct http_resp* res);

#endif