#define THREAD_COUNT 8

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

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
    pthread_cond_t work_cond;
    pthread_cond_t working_cond;

    size_t working_cnt;
    size_t thread_count;

    bool stop;
};

struct thread_pool_work *thread_pool_create_job(thread_func_t *job, void *arg)
{
    struct thread_pool_work *work;
    if (job == NULL)
    {
        return NULL;
    }

    work = malloc(sizeof(struct thread_pool_work));
    work->func = job;
    work->arg = arg;
    work->next = NULL;
}

void thread_pool_destroy_job(struct thread_pool_work *work)
{
    free(work);
    work = NULL;
}

struct thread_pool_work *thread_pool_get_work(struct thread_pool *tp)
{
    struct thread_pool_work *work;
    if (tp == NULL)
        return NULL;

    work = tp->first;
    if (work == NULL)
    {
        return NULL;
    }

    if (work->next == NULL)
    {
        tp->first = NULL;
        tp->last == NULL;
    }
    else
    {
        tp->first = work->next;
    }
    return work;
}

void tpool_worker(void *arg)
{
    struct thread_pool *tp = arg;
    struct thread_pool_work *work;

    while (1)
    {
        pthread_mutex_lock(&(tp->work_mutex));

        while (tp->first == NULL && !tp->stop)
        {
            pthread_cond_wait(&(tp->work_cond), &(tp->work_mutex));
        }
        if (tp->stop)
            break;

        work = thread_pool_get_work(tp);
        tp->working_cnt++;
        pthread_mutex_unlock(&(tp->work_mutex));
        if (work != NULL)
        {
            work->func(work->arg);
            thread_pool_destroy_job(work);
        }

        pthread_mutex_lock(&(tp->work_mutex));
        tp->working_cnt--;
        if (!tp->stop && tp->working_cnt == 0 && tp->first == NULL)
        {
            pthread_cond_signal(&(tp->working_cond));
        }
        pthread_mutex_unlock(&(tp->work_mutex));
    }

    tp->thread_count--;
    pthread_cond_signal(&(tp->working_cond));
    pthread_mutex_unlock(&tp->work_mutex);
}