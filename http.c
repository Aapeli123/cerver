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

void header_buf_free(struct http_header *headers, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(headers[i].key);
        free(headers[i].value);
    }
    free(headers);
}

void http_req_free(struct http_req *req)
{
    header_buf_free(req->headers, req->header_count);

    free(req->path);
    free(req->type);
    free(req->http_ver);
}

int http_parse_req(char *http_req, int len, struct http_req *req)
{
    struct http_header *header_buf;
    header_buf = (struct http_header *)calloc(HEADER_BUF_SIZE, sizeof(struct http_header));

    char *token = strtok(http_req, "\r\n");
    int reqline_len = strlen(token);
    char *req_line = (char *)(malloc(reqline_len * sizeof(char)));

    strcpy(req_line, token);
    printf("Request:\n%s\n\n", req_line);
    printf("Headers: \n");

    int hc = 0;
    while (1)
    {
        token = strtok(NULL, "\r\n");
        if (token == NULL)
            break;
        if (hc >= HEADER_BUF_SIZE - 1)
        {
            for (int i = 0; i < hc - 1; i++)
            {
                free(header_buf[i].key);
                free(header_buf[i].value);
            }
            free(header_buf);
            free(req_line);
            return 1;
        }
        int tokenlen = strlen(token);
        char *val_start = strchr(token, ':');
        int after_colon_len = strlen(val_start);
        val_start++;
        while (*val_start == ' ')
        {
            val_start++;
        }
        int val_len = strlen(val_start);
        char *val = (char *)malloc(val_len * sizeof(char));
        if (val == NULL)
        {
            printf("FUCK!!\n");
            return 1;
        }
        strcpy(val, val_start);
        int key_len = tokenlen - after_colon_len;

        char *key = (char *)malloc(key_len * sizeof(char));
        if (val == NULL)
        {
            printf("FUCK!!\n");
            return 1;
        }
        strncpy(key, token, key_len);

        header_buf[hc].key = key;
        header_buf[hc].value = val;
        printf("%s\n", token);
        hc++;
    }

    char *type, *path, *http_ver;

    char *r_type = strtok(req_line, " ");
    char *r_path = strtok(NULL, " ");
    char *r_http_ver = strtok(NULL, " ");

    type = (char *)malloc(strlen(r_type) * sizeof(char));
    path = (char *)malloc(strlen(r_path) * sizeof(char));
    http_ver = (char *)malloc(strlen(r_http_ver) * sizeof(char));

    strcpy(type, r_type);
    strcpy(path, r_path);
    strcpy(http_ver, r_http_ver);

    req->headers = header_buf;
    req->type = type;
    req->path = path;
    req->http_ver = http_ver;
    req->header_count = hc;
    return 0;
}

struct http_resp
{
    char *http_ver;
    int *status;
    char *reason_str;
    char *content;
};

int main()
{
    char buffer[463];
    FILE *f = fopen("./example_req.http", "r");
    fread(buffer, 463, 1, f);
    fclose(f);
    int header_count;
    struct http_req example_req;
    http_parse_req(buffer, 463, &example_req);
    http_req_free(&example_req);
    return 0;
}