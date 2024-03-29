#include "handler.h"

extern SSL_CTX* tls_context;
struct path_tok {
    char** list;
    int list_size;
};

static void free_path_tok(struct path_tok *pathTok) {
    for(int i = 0; i< pathTok->list_size; i++)
        free(pathTok->list[i]);
    free(pathTok->list);
    free(pathTok);
}

static struct path_tok* tokenize_path(char* path) {
    char *cur, *cur_token = NULL, *ctx = NULL;

    struct path_tok* tokenized = calloc(1, sizeof (struct path_tok));
    int i = 0;
    ctx = calloc(strlen(path) + 2, sizeof(char ));

    char** list = malloc(sizeof (char*));
    char** newList;
    while (1) {
        cur = strtok_r(path, "/", &path);
        if(cur == NULL){
            break;
        }
        strcat(ctx, "/");
        strcat(ctx, cur);

        cur_token = calloc(strlen(ctx) + 4, sizeof (char));
        strcpy(cur_token, ctx);
        strcat(cur_token, "/**");
        list[i] = cur_token;
        i++;
        newList = realloc(list, (i + 1) * sizeof (char*));
        if(newList == NULL) {
            free(list);
            return NULL;
        }
        list = newList;
    }
    free(ctx);
    tokenized->list_size = i;
    tokenized->list = list;
    return tokenized;
}

static char* resolve_redirect(char* path) {
    char* addr = (char*)hashmap_get(config->redirect_map, path);
    if(addr != NULL) return addr;


    char* path_wildcard = calloc(1, strlen(path) + 3);
    strcat(path_wildcard, path);
    strcat(path_wildcard, "/*");
    free(path_wildcard);

    addr = (char*)hashmap_get(config->redirect_map, path);
    if(addr != NULL) return addr;
    char path_clone[strlen(path) + 1];
    memset(path_clone, 0, strlen(path) + 1);
    strcpy(path_clone, path);
    // Resolve wildcard redirect
    struct path_tok* possible_paths = tokenize_path(path_clone);
    for (int i = (possible_paths->list_size - 1); i >= 0; i--) {
        char* attempt = possible_paths->list[i];
        addr = (char*)hashmap_get(config->redirect_map, attempt);
        if(addr != NULL) {
            free_path_tok(possible_paths);
            return addr;
        }
    }
    free_path_tok(possible_paths);
    return NULL;
}

static char* resolve_path(char* path) {
    char* content = (char*)hashmap_get(config->route_map, path);
    if(content!=NULL) return content;

    char* path_wildcard = calloc(1, strlen(path) + 3);
    strcat(path_wildcard, path);
    strcat(path_wildcard, "*");

    content = (char*)hashmap_get(config->route_map, path_wildcard);
    free(path_wildcard);
    if(content!=NULL) return content;

    char path_clone[strlen(path) + 1];
    memset(path_clone, 0, strlen(path) + 1);
    strcpy(path_clone, path);

    struct path_tok* possible_paths = tokenize_path(path_clone);
    for (int i = (possible_paths->list_size - 1); i >= 0; i--) {
        char* attempt = possible_paths->list[i];
        content = (char*)hashmap_get(config->route_map, attempt);
        if(content != NULL) {
            free_path_tok(possible_paths);
            return content;
        }
    }
    free_path_tok(possible_paths);
    return NULL;
}

static void respond_with_err(int client_fd, SSL* ssl) {
    char* resp = http_response_500(config->err_page);
    if(config->ssl) {
        SSL_write(ssl, resp, strlen(resp) + 1);
    } else {
        write_response(client_fd, resp, strlen(resp) + 1);
    }
    free(resp);
}

int handle_request(char *req_buffer, int req_size, int client_fd, SSL* ssl)
{
    if (req_buffer == NULL)
    {
        respond_with_err(client_fd, ssl);
        return 0;
    }
    struct http_req *req = (struct http_req *)calloc(1, sizeof(struct http_req));
    int r = http_parse_req(req_buffer, req_size, req);
    if (r < 0)
    {
        http_req_free(req);
        respond_with_err(client_fd, ssl);
        return -1;
    }

    char* resp;

    char* redirect = resolve_redirect(req->path);
    if(redirect != NULL) {
        resp = http_response_301(redirect);
        if(config->ssl) {
            SSL_write(ssl, resp, strlen(resp) + 1);
        } else {
            write_response(client_fd, resp, strlen(resp) + 1);
        }
        http_req_free(req);
        free(resp);
        return 0;
    }
    char* content = resolve_path(req->path);
    if(content == NULL) {
        content = config->fallback_page;
        int len = strlen(content);
        char content_len_char[128];
        sprintf(content_len_char, "%d", len);
        struct http_header headers[] = {{.key = "Content-Type", .value = "text/html"}, {.key = "Content-Length", .value = content_len_char}, {"Connection", "close"}};
        resp = http_response_404(content, headers, 3);
    } else {
        char* mime = mime_for_filename(req->path);
        int content_len = strlen(content);
        char content_len_char[128];
        sprintf(content_len_char, "%d", content_len);
        struct http_header headers[] = {{.key = "Content-Type", .value = mime}, {.key = "Content-Length", .value = content_len_char}, {"Connection", "close"}};
        resp = http_response_200(content, headers, 3);
    }

    if(resp == NULL) {
        free(resp);
        http_req_free(req);
        respond_with_err(client_fd, ssl);
        return -1;
    }
    if(config->ssl) {
        SSL_write(ssl, resp, strlen(resp) + 1);
    } else {
        write_response(client_fd, resp, strlen(resp) + 1);
    }
    free(resp);
    http_req_free(req);
    return 0;
}

void handler_worker(void *client_fd)
{
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
    int fd = (int)client_fd; // It's an int, source: "Trust me bro"
    char *buffer = (char *)calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        return;
    }
    int n;
    int i = 0;
    int bufSize = BUFFER_SIZE;
    do {
        if(bufSize >= 1024 * 1000) { // Max req size is 1000 KB
            respond_with_err(fd, NULL);
            shutdown(fd, SHUT_RDWR);
            free(buffer);
            printf("ERR: Req from fd %d was too long. Aborting...\n", fd);
            return;
        }
        char* new_buf = realloc(buffer, bufSize);
        if(new_buf == NULL) {
            respond_with_err(fd, NULL);
            shutdown(fd, SHUT_RDWR);
            free(buffer);
            return;
        }
        buffer = new_buf;
        n = read(fd, buffer + BUFFER_SIZE * sizeof(char) * i, BUFFER_SIZE);
        
        if (n == -1)
        {
            perror("ERR");
            respond_with_err(fd, NULL);
            shutdown(fd, SHUT_RDWR);
            free(buffer);
            return;
        }
        bufSize += BUFFER_SIZE;
        i++;
    } while(n == BUFFER_SIZE);
    

    handle_request(buffer, bufSize, fd, NULL);
    shutdown(fd, SHUT_RDWR);
    close(fd);
    free(buffer);
    // free(fd);
}


void handler_ssl_worker(void* client_fd) {

    int fd = (int)client_fd; // It's an int, source: "Trust me bro"
    SSL* ssl;
    ssl = SSL_new(tls_context);
    SSL_set_fd(ssl, fd);
    if(SSL_accept(ssl) <= 0) {
        SSL_free(ssl);
        close(fd);
        return;
    }
    char *buffer = (char *)calloc(1, BUFFER_SIZE);
    if (buffer == NULL)
    {
        respond_with_err(fd, ssl);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(fd);

        return;
    }
    int n;
    int i = 0;
    int bufSize = BUFFER_SIZE;
    do {
        if(bufSize >= 1024 * 1000) { // Max req size is 1000 KB
            respond_with_err(fd, ssl);

            SSL_shutdown(ssl);
            free(buffer);
            printf("ERR: Req from fd %d was too long. Aborting...\n", fd);
            return;
        }
        char* new_buf = realloc(buffer, bufSize);
        if(new_buf == NULL) {
            respond_with_err(fd, ssl);

            SSL_shutdown(ssl);
            free(buffer);
            return;
        }
        buffer = new_buf;
        n = SSL_read(ssl, buffer + BUFFER_SIZE * sizeof(char) * i, BUFFER_SIZE);

        if (n == -1)
        {
            perror("ERR");
            respond_with_err(fd, ssl);

            SSL_shutdown(ssl);
            free(buffer);
            return;
        }
        bufSize += BUFFER_SIZE;
        i++;
    } while(n == BUFFER_SIZE);

    handle_request(buffer, bufSize, 0, ssl);
    free(buffer);
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(fd);
}