#include <signal.h>

#include "server.h"
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
    http_clean_mimetable();
}

void handle_sigint()
{
    printf("SIGINT or SIGTERM Recieved, exiting cleanly!\n");
    clean();
    exit(0);
}

int main()
{;
    config = config_create();
    config_read("./test.config", config);

    printf("Starting Cerver...\n");
    init_content_type_table();
    thread_pool = thread_pool_create(config->threads);
    printf("Thread pool created\n");

    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
    server_start(config, thread_pool);
    thread_pool_destroy(thread_pool);
    http_clean_mimetable();
    config_destroy(config);
    return 0;
}
