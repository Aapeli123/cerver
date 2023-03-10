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
bool should_run = true;
SSL_CTX *tls_context;
pthread_mutex_t* tls_context_mutex;
void write_data(int fd) {

}

void read_data(int fd) {

}

void server_cleanup()
{
    printf("Cleaning up...\n");
    printf("Shutting down server socket\n");
    shutdown(server_descriptor, SHUT_RDWR);
    close(server_descriptor);
    if(config->ssl) {
        SSL_CTX_free(tls_context);
        pthread_mutex_destroy(tls_context_mutex);
        free(tls_context_mutex);
    }

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
    int err = listen(server_descriptor, 512);
    return err;
}

int init()
{
    if(!config->ssl) return 0; // Don't allocate ssl context if ssl is not enabled:
    printf("Enabling TLS\n");
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *method = TLS_server_method();
    tls_context = SSL_CTX_new(method);
    if ( tls_context == NULL )
    {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    if(SSL_CTX_use_certificate_file(tls_context, "./public.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    if(SSL_CTX_use_PrivateKey_file(tls_context, "./private.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    if ( !SSL_CTX_check_private_key(tls_context) )
    {
        printf( "Private key does not match the public certificate\n");
        return -1;
    }
    tls_context_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(tls_context_mutex, NULL);
    printf("TLS Enabled\n");

    return 0;
}

int server_start(struct thread_pool *tp)
{
    HANDLE_ERROR(init());

    // Start the server:
    HANDLE_ERROR(server_create_socket());
    HANDLE_ERROR(server_bind(config->port));
    HANDLE_ERROR(server_listen());

    printf("Server listening on port %d\n", config->port);

    // Accept connections
    struct sockaddr_in client;
    int client_addr_size = sizeof(struct sockaddr_in);

    while (should_run)
    {
        // client_fd = malloc(sizeof(int));
        int client_fd = accept(server_descriptor, (struct sockaddr *)&client, (socklen_t *)&client_addr_size);

        if (client_fd < 0)
        {
            perror("ERR: ");
            continue;
        }
        if(config->ssl) {
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
            thread_pool_add_work(tp,handler_ssl_worker, (void*)client_fd);
            continue;
        }

#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
        thread_pool_add_work(tp, handler_worker, (void *)client_fd);
    }

    server_cleanup();
    return 0;
}