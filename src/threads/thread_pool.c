#define THREAD_COUNT 8

#include <pthread.h>
#include <stdbool.h>

struct thread_pool_thread
{
    pthread_t thread_id;
    pthread_mutex_t mutex;
};


struct thread_pool_thread threads[THREAD_COUNT];

void que_job() {

}