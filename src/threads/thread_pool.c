#include "thread_pool.h"

struct thread_pool_work *thread_pool_create_job(thread_func_t job, void *arg)
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
    return work;
}

static void thread_pool_destroy_job(struct thread_pool_work *work)
{
    free(work);
    work = NULL;
}

static struct thread_pool_work *thread_pool_get_work(struct thread_pool *tp)
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
        tp->last = NULL;
    }
    else
    {
        tp->first = work->next;
    }
    return work;
}

static void *tpool_worker(void *arg)
{
    struct thread_pool *tp = arg;
    struct thread_pool_work *work;

    while (1)
    {
        pthread_mutex_lock(tp->work_mutex);

        while (tp->first == NULL && !tp->stop)
        {
            pthread_cond_wait(tp->work_cond, tp->work_mutex);
        }
        if (tp->stop)
            break;

        work = thread_pool_get_work(tp);
        tp->working_cnt++;
        pthread_mutex_unlock(tp->work_mutex);
        if (work != NULL)
        {
            work->func(work->arg);
            thread_pool_destroy_job(work);
        }

        pthread_mutex_lock(tp->work_mutex);
        tp->working_cnt--;
        if (!tp->stop && tp->working_cnt == 0 && tp->first == NULL)
        {
            pthread_cond_signal(tp->working_cond);
        }
        pthread_mutex_unlock(tp->work_mutex);
    }

    tp->thread_count--;
    pthread_cond_signal(tp->working_cond);
    pthread_mutex_unlock(tp->work_mutex);
    return 0;
}

struct thread_pool *thread_pool_create(int threads)
{
    printf("Creating a thread pool with %d threads\n", threads);
    struct thread_pool *tp;
    pthread_t thread;

    if (threads == 0)
    {
        threads = DEFAULT_THREAD_COUNT;
    }

    tp = calloc(1, sizeof(struct thread_pool));
    tp->thread_count = threads;


    tp->work_mutex = malloc(sizeof(pthread_mutex_t));
    tp->work_cond = malloc(sizeof(pthread_cond_t));
    tp->working_cond = malloc(sizeof(pthread_cond_t));

    pthread_mutex_init(tp->work_mutex, NULL);
    pthread_cond_init(tp->work_cond, NULL);
    pthread_cond_init(tp->working_cond, NULL);

    tp->first = NULL;
    tp->last = NULL;

    for (size_t i = 0; i < threads; i++)
    {
        pthread_create(&thread, NULL, tpool_worker, tp);
        pthread_detach(thread);
    }
    return tp;
}

void thread_pool_add_work(struct thread_pool *tp, thread_func_t func, void *arg)
{
    struct thread_pool_work *work;

    work = thread_pool_create_job(func, arg);

    pthread_mutex_lock(tp->work_mutex);
    if (tp->first == NULL)
    {
        tp->first = work;
        tp->last = tp->first;
    }
    else
    {
        tp->last->next = work;
        tp->last = work;
    }
    pthread_cond_broadcast(tp->work_cond);
    pthread_mutex_unlock(tp->work_mutex);
}

void thread_pool_wait(struct thread_pool *tp)
{
    if (tp == NULL)
        return;

    pthread_mutex_lock(tp->work_mutex);
    while (1)
    {
        if ((!tp->stop && tp->working_cnt != 0) || (tp->stop && tp->thread_count != 0))
        {
            pthread_cond_wait(tp->working_cond, tp->work_mutex);
        }
        else
        {
            break;
        }
    }
    pthread_mutex_unlock(tp->work_mutex);
}

void thread_pool_destroy(struct thread_pool *tp)
{
    struct thread_pool_work *work;
    struct thread_pool_work *work2;

    if (tp == NULL)
        return;

    pthread_mutex_lock(tp->work_mutex);
    work = tp->first;
    while (work != NULL)
    {
        work2 = work->next;
        thread_pool_destroy_job(work);
        work = work2;
    }
    tp->stop = true;
    pthread_cond_broadcast(tp->work_cond);
    pthread_mutex_unlock(tp->work_mutex);

    thread_pool_wait(tp);

    pthread_mutex_destroy(tp->work_mutex);
    pthread_cond_destroy(tp->work_cond);
    pthread_cond_destroy(tp->working_cond);

    free(tp->work_mutex);
    free(tp->work_cond);
    free(tp->working_cond);
    free(tp);
}