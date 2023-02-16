#include <signal.h>

#include "server.h"
#include "http.h"
#include "threads/thread_pool.h"

struct thread_pool* thread_pool;

void clean() {
    server_cleanup();
    if(thread_pool != NULL)
        thread_pool_destroy(thread_pool);
}

void handle_sigint() {
    printf("SIGINT or SIGTERM Recieved, exiting cleanly!\n");
    clean();
    exit(0);
}

void handle_segv() {
    printf("Oh shit i think this should have not happened :(\n");
    clean();
    exit(SIGSEGV);
}

int main()
{
    printf("Starting Cerver...\n");
    thread_pool = thread_pool_create(1);
    printf("Thread pool created\n");

    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
    signal(SIGSEGV, handle_segv);
    server_start(8080, thread_pool);
    thread_pool_destroy(thread_pool);
    return 0;
}
