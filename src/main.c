#include "server.h"
#include "http.h"
#include <unistd.h>
#include "threads/thread_pool.h"


int main()
{
    struct thread_pool* thread_pool = thread_pool_create(24);
    server_start(8080);
    thread_pool_destroy(thread_pool);
    return 0;
}
