#include <signal.h>

#include "server.h"
#include "http.h"
#include "threads/thread_pool.h"

struct thread_pool* thread_pool;


void handle_sigint() {
    printf("SIGINT or SIGTERM Recieved, exiting cleanly!\n");
    server_cleanup();
    if(thread_pool != NULL)
        thread_pool_destroy(thread_pool);
    exit(0);
}

int main()
{
    printf("Starting Cerver...\n");
    thread_pool = thread_pool_create(24);
    printf("Thread pool created\n");

    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);

    server_start(8080, thread_pool);
    thread_pool_destroy(thread_pool);
    return 0;
}
