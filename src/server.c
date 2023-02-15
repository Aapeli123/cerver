#include "server.h"


#define HANDLE_ERROR(a)   \
    if (a == -1)          \
    {                     \
        perror("ERR");    \
        server_cleanup(); \
        return -1;        \
    }

// Globals for server state:
int server_descriptor;
struct sockaddr_in server_addr;
char *buffer;
SSL_CTX *tls_context;

void server_cleanup()
{
    printf("Cleaning up...\n");
    printf("Shutting down server socket\n");
    shutdown(server_descriptor, SHUT_RDWR);
    printf("Freeing buffer...\n");
    free(buffer);

    SSL_CTX_free(tls_context);
}

int server_create_socket()
{
    server_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    HANDLE_ERROR(server_descriptor);
    int optval = 1;
    int err = setsockopt(server_descriptor, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    return err;
}

int server_bind(int port)
{
    // Configure server_addr struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    // Bind socket
    int err = bind(server_descriptor, (struct sockaddr *)&server_addr, sizeof(server_addr));
    return err;
}

int server_listen()
{
    int err = listen(server_descriptor, 3);
    return err;
}

int init()
{
    printf("Allocating buffer\n");
    buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
    if(buffer == NULL) {
        return -1;
    }
    tls_context = SSL_CTX_new(TLS_method());
    if(tls_context == NULL) {
        return -1;
    }
    return 0;
}

int server_start(int port) {
    HANDLE_ERROR(init());
    printf("Starting cerver\n");

    // Start the server:
    HANDLE_ERROR(server_create_socket());
    HANDLE_ERROR(server_bind(port));
    HANDLE_ERROR(server_listen());

    // Accept connections
    struct sockaddr_in client;
    int client_fd, n, client_addr_size = sizeof(struct sockaddr_in);

    client_fd = accept(server_descriptor, (struct sockaddr *)&client, (socklen_t *)&client_addr_size);

    n = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (n == -1)
    {
        perror("ERR");
        shutdown(client_fd, SHUT_RDWR);
        server_cleanup();
    }
    handle_request(buffer, n, client_fd);

    shutdown(client_fd, SHUT_RDWR);
    server_cleanup();
    return 0;
}