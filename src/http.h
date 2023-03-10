#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hashmap/hashmap.h"

#define HEADER_BUF_SIZE 2048

static hashmap_t* CONTENT_TYPES = NULL;

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
char* http_response_200(char* content, struct http_header* headers, int header_count);
void init_content_type_table();
void http_clean_mimetable();
char* get_content_type(char* filetype);
#endif