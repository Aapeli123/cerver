#include <signal.h>

#include "server/server.h"
#include "mime/mime.h"
#include "threads/thread_pool.h"
#include "config/config.h"
threadpool_t *thread_pool = NULL;
config_t* config = NULL;

void clean()
{
    server_cleanup();
    if (thread_pool != NULL)
        thread_pool_destroy(thread_pool);
    if(config != NULL)
        config_destroy(config);
    mime_table_destroy();
}

void handle_sigint()
{
    printf("SIGINT or SIGTERM Recieved, exiting cleanly!\n");
    clean();
    exit(0);
}

int main()
{
    printf("Starting Cerver...\n");

    // Initialize lookup table for content types:
    mime_table_init();

    // Read the config file and cache relevant data
    config = config_create();
    config_read("../../test.config", config);

    // Start a thread pool
    thread_pool = thread_pool_create(config->threads);
    printf("Thread pool created\n");

    // Bind signal handlers for clean exit
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
    signal(SIGSTOP, handle_sigint);

    signal(SIGPIPE, SIG_IGN);


    // Start the server
    server_start(thread_pool);

    // If somehow the server stops its loop, clean memory
    thread_pool_destroy(thread_pool);
    config_destroy(config);
    return 0;
}
