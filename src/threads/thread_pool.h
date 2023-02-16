#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define DEFAULT_THREAD_COUNT 4

typedef void (*thread_func_t)(void *arg);

struct thread_pool_work
{
    thread_func_t func;
    void *arg;
    struct thread_pool_work *next;
};

struct thread_pool
{
    struct thread_pool_work *first;
    struct thread_pool_work *last;

    pthread_mutex_t *work_mutex;
    pthread_cond_t* work_cond;
    pthread_cond_t* working_cond;

    size_t working_cnt;
    size_t thread_count;

    bool stop;
};

struct thread_pool* thread_pool_create(int threads);
void thread_pool_wait(struct thread_pool* tp);
void thread_pool_destroy(struct thread_pool* tp);
struct thread_pool_work *thread_pool_create_job(thread_func_t job, void *arg);
void thread_pool_add_work(struct thread_pool* tp, thread_func_t func, void* arg);

#endif