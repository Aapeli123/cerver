#include "http.h"

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

    free(req);
}

int http_parse_req(char *http_req, int len, struct http_req *req)
{
    if (http_req == NULL)
    {
        return -1;
    }
    struct http_header *header_buf;
    header_buf = (struct http_header *)calloc(1, HEADER_BUF_SIZE);
    char *http_req_save_ptr = NULL;
    char *token = strtok_r(http_req, "\r\n", &http_req_save_ptr);
    if (token == NULL)
    {
        header_buf_free(header_buf, 0);
        return -1;
    }
    int reqline_len = (int)strlen(token) + 1;
    char *req_line = (char *)(malloc(reqline_len * sizeof(char)));

    strcpy(req_line, token);

    int hc = 0;
    while (1)
    {
        token = strtok_r(http_req_save_ptr, "\r\n", &http_req_save_ptr);
        if (token == NULL)
            break;
        if (hc >= HEADER_BUF_SIZE - 1)
        {
            header_buf_free(header_buf, hc);
            free(req_line);
            return 1;
        }
        int tokenlen = strlen(token) + 1;
        char *val_start = strchr(token, ':');
        int after_colon_len = strlen(val_start) + 1;
        val_start++;
        while (*val_start == ' ')
        {
            val_start++;
        }
        int val_len = strlen(val_start) + 1;
        char *val = (char *)malloc(val_len * sizeof(char));
        if (val == NULL)
        {
            header_buf_free(header_buf, hc);
            printf("FUCK!!\n");
            return 1;
        }
        strcpy(val, val_start);
        int key_len = tokenlen - after_colon_len;

        char *key = (char *)malloc(key_len * sizeof(char));
        if (val == NULL)
        {
            header_buf_free(header_buf, hc);

            printf("FUCK!!\n");
            return 1;
        }
        strncpy(key, token, key_len);

        header_buf[hc].key = key;
        header_buf[hc].value = val;
        hc++;
    }

    char *type, *path, *http_ver;
    char *save_ptr;
    char *r_type = strtok_r(req_line, " ", &save_ptr);
    char *r_path = strtok_r(save_ptr, " ", &save_ptr);
    char *r_http_ver = strtok_r(save_ptr, " ", &save_ptr);

    type = (char *)malloc((strlen(r_type) + 1) * sizeof(char));
    path = (char *)malloc((strlen(r_path) + 1) * sizeof(char));
    http_ver = (char *)malloc((strlen(r_http_ver) + 1) * sizeof(char));

    strcpy(type, r_type);
    strcpy(path, r_path);
    strcpy(http_ver, r_http_ver);

    req->headers = header_buf;
    req->type = type;
    req->path = path;
    req->http_ver = http_ver;
    req->header_count = hc;

    free(req_line);
    return 0;
}

char *http_stringify_headers(struct http_header *headers, int header_count, int *headers_size)
{
    int headers_len = 0;
    // First calculate bytes needed:
    // add the header strings lengths to the variable
    for (int i = 0; i < header_count; i++)
    {
        headers_len += strlen(headers[i].key);
        headers_len += strlen(headers[i].value);
    }
    // add the colon and whitespace part of the header for each header
    headers_len += header_count * 4 * sizeof(char) + 2; // multiply by 2 to add the ending \r\n
    headers_len += 1;                                   // for the null terminator
    char *header_string = (char *)calloc(1, headers_len);

    for (int i = 0; i < header_count; i++)
    {
        int key_len = strlen(headers[i].key);
        int val_len = strlen(headers[i].value);
        char header_str[key_len + val_len + 5];
        sprintf(header_str, "%s: %s\r\n", headers[i].key, headers[i].value);
        strcat(header_string, header_str);
    }
    strcat(header_string, "\r\n");
    *headers_size = headers_len;
    return header_string;
}

char *http_stringify_resp(struct http_resp *res)
{
    int status_len = strlen(res->http_ver) + strlen(res->reason_str) + 8; // 8 comes form: 1 (terminating null byte) + 3 (status code) + 2 (whitespace) + 2 (http linebreak)
    char *status_line = (char *)malloc(status_len);

    sprintf(status_line, "%s %d %s\r\n", res->http_ver, res->status, res->reason_str);

    int headers_size;

    char *headers = http_stringify_headers(res->headers, res->header_count, &headers_size);
    int content_len = strlen(res->content) + 1;

    int resp_size = headers_size + content_len + strlen(status_line);

    char *response = calloc(1, resp_size);
    strncat(response, status_line, status_len);
    strncat(response, headers, headers_size);
    strncat(response, res->content, content_len);

    free(headers);
    free(status_line);

    return response;
}

void init_content_type_table() {
    CONTENT_TYPES = hashmap_create();
#define ADD_TYPE(filetype, content_type) hashmap_add(CONTENT_TYPES, filetype, content_type)
    ADD_TYPE("html","text/html");
    ADD_TYPE("js","text/javascript");
    ADD_TYPE("css","text/css");
    ADD_TYPE("txt","text/plain");
    ADD_TYPE("json","application/json");
    ADD_TYPE("jpg","image/jpeg");
    ADD_TYPE("png","image/png");
    ADD_TYPE("pdf","application/pdf");
#undef ADD_TYPE
}

char* get_content_type(char* filetype) {
    return hashmap_get(CONTENT_TYPES, filetype);
}

void http_clean_mimetable() {
    if(CONTENT_TYPES != NULL)
        hashmap_destroy(CONTENT_TYPES);
}

// Get the http response string for 200 OK with the content 'content'
char* http_response_200(char* content, struct http_header* headers, int header_count) {
    struct http_resp res = {.content = content, .header_count = header_count, .headers = headers, .http_ver = "HTTP/1.1", .status = 200, .reason_str = "OK"};
    return http_stringify_resp(&res);
}