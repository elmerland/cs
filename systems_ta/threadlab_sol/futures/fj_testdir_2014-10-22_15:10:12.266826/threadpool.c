#include "threadpool.h"
#include "list.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

struct worker_queue {
    int depth;
    pthread_t thread;
    
    struct thread_pool* pool;

    pthread_mutex_t lock;
    struct list     queue;
};

struct thread_pool {
    int num_workers;
    struct worker_queue* workers;

    pthread_mutex_t lock;
    pthread_cond_t  signal; 
    struct list queue;

    bool shutdown;
};

enum future_state {
    NOT_EVALUATED,
    EVALUATING,
    FINISHED
};

struct future {
    int depth;
    enum future_state state;

    struct worker_queue* worker;
    struct worker_queue* creator;

    fork_join_task_t work;
    void*       arg;
    void*       retn;

    struct list_elem elem;

    sem_t signal;
};


static int
max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}
    
static __thread struct worker_queue* current_worker = NULL;

static void init_worker(struct thread_pool* pool, struct worker_queue* worker) {
    worker->depth = 0;
    worker->pool = pool;

    list_init(&worker->queue);
    pthread_mutex_init(&worker->lock, NULL);
}

static void execute_future_locked(pthread_mutex_t* lock,
                           struct worker_queue* worker,
                           struct future* future) {
//    assert(future->state == NOT_EVALUATED);

    future->state = EVALUATING;
    future->worker = worker;

    int depth = worker->depth;
    worker->depth = future->depth;
    pthread_mutex_unlock(lock);
    
    future->retn = future->work(worker->pool, future->arg);
    worker->depth = depth;

    future->state = FINISHED;
    sem_post(&future->signal);
}

static void process_self(struct worker_queue* worker) {
    pthread_mutex_lock(&worker->lock);
    while (!list_empty(&worker->queue)) {
        struct list_elem* e = list_pop_front(&worker->queue);
        struct future* future = list_entry(e, struct future, elem);

        execute_future_locked(&worker->lock, worker, future);
        pthread_mutex_lock(&worker->lock);
    }
    pthread_mutex_unlock(&worker->lock);
}
static bool process_global(struct worker_queue* worker) {
    pthread_mutex_lock(&worker->pool->lock);
    if (!list_empty(&worker->pool->queue)) {
        struct list_elem* e = list_pop_back(&worker->pool->queue);
        struct future* future = list_entry(e, struct future, elem);
        future->creator = worker;
        execute_future_locked(&worker->pool->lock, worker, future);
        return true;
    }
    pthread_mutex_unlock(&worker->pool->lock);
    return false;
}

static struct future* steal_future_locked(struct worker_queue* other, int depth) {
    struct list_elem* e;
    for (e = list_begin(&other->queue); e != list_end(&other->queue); e = list_next(e)) {
        struct future* future = list_entry(e, struct future, elem);
        if (future->state == NOT_EVALUATED && future->depth > depth) {
            return future;
        }
    }
    return NULL;
}
static void* worker_loop(void* arg) {
    struct worker_queue* worker = (struct worker_queue*)arg;
    struct thread_pool* pool = worker->pool;
    current_worker = worker;
 outer:
    while (true) {
        process_self(worker);
        if (process_global(worker)) {
            continue;
        }
        int i;
        for (i = 0; i < pool->num_workers; i++) {
            struct worker_queue* other = &pool->workers[i];
            if (other == worker) {
                continue;
            }
            pthread_mutex_lock(&other->lock);
            struct future* future = steal_future_locked(other, 0);
            if (future != NULL) {
                list_remove(&future->elem);
                execute_future_locked(&other->lock, worker, future);
                goto outer;
            }
            else {
                pthread_mutex_unlock(&other->lock);
            }
        }
        pthread_mutex_lock(&pool->lock);
        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->lock);
            return NULL;
        }
        if (list_empty(&pool->queue)) {
            pthread_cond_wait(&pool->signal, &pool->lock);
            if (pool->shutdown) {
                pthread_mutex_unlock(&pool->lock);
                return NULL;
            }
        }
        pthread_mutex_unlock(&pool->lock);
    }
}

static void start_worker(struct worker_queue* worker) {
    if (pthread_create(&worker->thread, NULL, worker_loop, worker) != 0) {
        perror("pthread create: ");
        abort();
    } 
}
struct thread_pool* thread_pool_new(int nthreads) {
    struct thread_pool* pool = malloc(sizeof(struct thread_pool));
    pool->num_workers = nthreads;
    pool->shutdown = false;
    pool->workers = calloc(pool->num_workers, sizeof(struct worker_queue));
    list_init(&pool->queue);
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->signal, NULL);

    int i;
    for (i = 0; i < pool->num_workers; i++) {
        init_worker(pool, &pool->workers[i]);
    }
    for (i = 0; i < pool->num_workers; i++) {
        start_worker(&pool->workers[i]);
    }
    return pool;
}

void thread_pool_shutdown_and_destroy(struct thread_pool* pool) {
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = true;
    pthread_cond_broadcast(&pool->signal);
    pthread_mutex_unlock(&pool->lock);

    int i;
    for (i = 0; i < pool->num_workers; i++) {
        void* retn;
        pthread_join(pool->workers[i].thread, &retn);
        pthread_mutex_destroy(&pool->workers[i].lock);
    }
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->signal);
    free(pool->workers);
    free(pool);
}

struct future* thread_pool_submit(struct thread_pool* pool, fork_join_task_t work, void* arg) {
    struct future* future = malloc(sizeof(struct future));
    future->work = work;
    future->arg = arg;
    future->state = NOT_EVALUATED;
    future->worker = NULL;
    sem_init(&future->signal, 0, 0);

    future->creator = current_worker;
    if (current_worker == NULL) {
        future->depth = 0;
        pthread_mutex_lock(&pool->lock);
        list_push_front(&pool->queue, &future->elem);
        pthread_cond_signal(&pool->signal);
        pthread_mutex_unlock(&pool->lock);
    }
    else {
        future->depth = current_worker->depth;
        pthread_mutex_lock(&current_worker->lock);
        list_push_front(&current_worker->queue, &future->elem);
        pthread_mutex_unlock(&current_worker->lock);

        pthread_mutex_lock(&pool->lock);
        pthread_cond_signal(&pool->signal);
        pthread_mutex_unlock(&pool->lock);
    }
    return future;
}

static void* wait_for_future(struct future* future) {
    sem_wait(&future->signal);
    assert(future->state == FINISHED);
    return future->retn;
}

void* future_get(struct future* future) {
    if (current_worker == NULL || future->state == FINISHED) {
        return wait_for_future(future);
    }

    struct worker_queue* worker = current_worker;
    struct worker_queue* creator = future->creator;
    
    pthread_mutex_lock(&creator->lock);
    if (future->state == NOT_EVALUATED) {
        future->depth = max(future->depth, worker->depth + 1);

        list_remove(&future->elem);
        execute_future_locked(&creator->lock, worker, future);
        return future->retn;
    }
    else if (future->state == FINISHED) {
        pthread_mutex_unlock(&creator->lock);
        return future->retn;
    }
    while (future->state == EVALUATING) {
        int leapfrog_depth = max(future->depth, worker->depth);
        struct future* stolen = steal_future_locked(creator, leapfrog_depth);
        if (stolen != NULL) {
            stolen->depth = leapfrog_depth;
            list_remove(&stolen->elem);
            execute_future_locked(&creator->lock, worker, future);
            pthread_mutex_lock(&creator->lock);
            continue;
        }

        if (future->state == EVALUATING) {
            pthread_mutex_unlock(&creator->lock);
            // well nothing left to do now
            return wait_for_future(future);
        }
    }
    pthread_mutex_unlock(&creator->lock);
    assert(future->state == FINISHED);
    return future->retn;
}

void future_free(struct future* future) {
    sem_destroy(&future->signal);
    free(future);
}
