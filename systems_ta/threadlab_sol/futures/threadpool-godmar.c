/*
 * Fork/Join Thread pool sample solution.
 *
 * DO NOT PUBLISH.
 *
 * Written by Godmar Back (gback@cs.vt.edu), Fall 2014 CS 3214.
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <assert.h>

#include "threadpool.h"
#include "threadpool_lib.h"
#include "list.h"

#define max(a, b)    ((a) > (b) ? (a) : (b))

/* child-stealing has bad stack growth bounds, and for
 * simpson integration with deep nesting we can easily
 * hit them.  Default is 2MB, set to 128MB */
#define WORKER_STACK_SIZE 128*1024*1024
#define MAX_FUTURE_FREE_LIST_LENGTH 16
#define KEEP_STATS              1

/* This does not help at all and causes tremendous variations
 * in run time. */
#undef TRY_RANDOM_STEALING

//
// returns pointer to struct worker * if a thread is a pool worker
// global so that it can be used from future_get() to distinguish
// external joins from internal ones
//
static __thread struct worker * current_worker;

struct thread_pool {
    pthread_mutex_t lock;       // protects remaining members
    pthread_cond_t new_submissions;        // signals additions to submission queue
                                // also used to get idle threads to look for steals again
    int nthreads;               // total number of active threads
    struct worker * workers;    // array of worker threads that make this pool
    struct list submissionqueue; // global (external) submission queue

    bool shutdown;              // true if threads should quit after finishing their queues
};

struct worker {
    pthread_mutex_t lock;       // protects remaining members
    struct list queue;          // local queue, serviced in LIFO order by local spawns

    struct thread_pool * pool;  // containing pool
    pthread_t thread;           // actual pthreads
    unsigned long srand;        // per-thread srand for random stealing
    int current_depth;          // current depth in DAG, see Leapfrogging paper
#ifdef KEEP_STATS
    int taskprocessed;          // counter of tasks processed
    int taskstolen;             // counter of tasks stolen from other workers
    int taskleapfrogged;        // counter of tasks leapfrogged from holder
    int globaltasks;            // counter of tasks taken from global queue
    int futurereuse;            // how often I avoided malloc() in _submit
    int tasksubmitted;          // total number of internal submissions
    int unabletohelp;           // could not help or steal in future_get
    int joininprogressfuture;   // how often a future was joined that was already executing
#endif
    struct list free_futures;   // free list of futures for custom allocator
    int nfree_futures;          // number of elements in list
};

enum task_state 
{
    FUTURE_NEW,                 // NEW -> INPROGRESS -> DONE
    FUTURE_INPROGRESS,
    FUTURE_DONE
};

struct future {
    // Most members in future are protected by owner's lock if owner != NULL,
    // else by pool->lock
    struct list_elem elem; 

    fork_join_task_t func;
    void * calldata;
    void * result;  

    enum task_state state;
    int depth;              // depth in DAG, ==0 for externally submitted tasks
    struct worker * owner; // pointer to worker in whose queue this future
                           // currently resides; set up submission (for internal tasks)
                           // or when pulled from global queue.
    struct worker * executor; // pointer to worker who is actually running
                            // this future in case it was stolen
                            //
    pthread_mutex_t lock;   // protects only isdone and statechange 
    bool    isdone;
    pthread_cond_t statechange; // if joiner(s) are blocked, used to signal them
};

/* Create a new thread pool with nthreads */
struct thread_pool * 
thread_pool_new(int nthreads)
{
    int i;
    struct thread_pool * pool = calloc(sizeof * pool, 1);
    if (pool == NULL)
        perror("calloc"), exit(-1);

    pool->workers = calloc(nthreads, sizeof pool->workers[0]);
    if (pool->workers == NULL)
        perror("calloc"), exit(-1);

    pool->nthreads = nthreads;
    list_init(&pool->submissionqueue);
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->new_submissions, NULL);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, WORKER_STACK_SIZE);

#if 0
    size_t stacksize;
    pthread_attr_getstacksize(&attr, &stacksize);
    printf("default stack size is: %ld\n", stacksize);
#endif

    for (i = 0; i < nthreads; i++) {
        struct worker * worker = &pool->workers[i];
        list_init(&worker->queue);
        list_init(&worker->free_futures);
        worker->nfree_futures = 0;
        worker->srand = i;
        worker->pool = pool;
        pthread_mutex_init(&worker->lock, NULL);
    }

    for (i = 0; i < nthreads; i++) {
        struct worker * worker = &pool->workers[i];
        pthread_create(&worker->thread, &attr, worker_loop, worker);
    }
    return pool;
}

/* Submit a callable to FJ pool, returns future */
struct future * 
thread_pool_submit(struct thread_pool * pool, 
                   fork_join_task_t callable, 
                   void *callable_data)
{
    struct future * f;
    struct worker * worker = current_worker;
    if (worker == NULL || worker->nfree_futures == 0) {
        f = malloc(sizeof *f);
    } else {
#ifdef KEEP_STATS
        worker->futurereuse++;
#endif
        worker->nfree_futures--;
        f = list_entry(list_pop_front(&worker->free_futures), struct future, elem);
    }

    f->func = callable;
    f->calldata = callable_data;
    f->state = FUTURE_NEW;
    f->isdone = false;
    pthread_mutex_init(&f->lock, NULL);
    pthread_cond_init(&f->statechange, NULL);

    executor_submit(pool, f);
    return f;
}

/* Submit a task, either externally or internally */
static void
executor_submit(struct thread_pool * pool, struct future *f)
{
    if (current_worker != NULL) {
        pthread_mutex_lock(&current_worker->lock);
        f->depth = current_worker->current_depth + 1;
        f->owner = current_worker;
        list_push_front(&current_worker->queue, &f->elem);
        pthread_cond_signal(&pool->new_submissions);
        pthread_mutex_unlock(&current_worker->lock);
        // http://valgrind.10908.n7.nabble.com/valgrind-r11703-branches-HGDEV2-helgrind-td32383.html
        // Anybody who writes code that signals on a CV without holding 
        // the associated MX needs to be shipped off to a lunatic asylum 
        // ASAP, even though POSIX doesn't actually declare such behaviour 
        // illegal -- it makes code extremely difficult to understand/ 
        // reason about.  In particular it puts the signalling thread in 
        // a situation where it is racing against the released waiter 
        // as soon as the signalling is done, and so there needs to be 
        // some auxiliary synchronisation mechanism in the program that 
        // makes this safe -- or the race(s) need to be harmless, or 
        // probably nonexistent. 
        // 
        // Wake up potential stealers.
        // elided: pthread_mutex_lock(&pool->lock);
        // pthread_cond_signal(&pool->new_submissions);
        // elided: pthread_mutex_unlock(&pool->lock);
#ifdef KEEP_STATS
        current_worker->tasksubmitted++;
#endif
    } else {
        pthread_mutex_lock(&pool->lock);
        list_push_front(&pool->submissionqueue, &f->elem);
        f->depth = 0;
        f->owner = NULL;
        pthread_cond_signal(&pool->new_submissions);
        pthread_mutex_unlock(&pool->lock);
    }
}

/* run a task, without holding the lock associated with the task's owner */
static void * run_task(struct worker * worker, struct future *f) 
{
    int olddepth = worker->current_depth;
    worker->current_depth = f->depth;
    void * result = f->result = f->func(worker->pool, f->calldata);
#if KEEP_STATS
    worker->taskprocessed++;
#endif

    pthread_mutex_lock(&f->lock);
    f->isdone = true;
    f->state = FUTURE_DONE;
    __sync_synchronize();   // ensure f->result is in memory when state is FUTURE_DONE
    pthread_cond_broadcast(&f->statechange);
    pthread_mutex_unlock(&f->lock);

    worker->current_depth = olddepth;
    return result;
}

/* Attempt to remove a task from a queue, either its front or back. 
 * Used for local and global submission queue with different
 * list_pop_func.  Not used for stealing.
 *
 * Claim the task for execution after removing it from the list
 * set status to FUTURE_INPROGRESS and executor.
 */ 
static bool 
thread_pool_get_task_locked(struct worker *executor,
                     struct list *taskqueue, 
                     struct list_elem * (* list_pop_func)(struct list *), 
                     struct future **of)
{
    bool havetask = false;

    if (!list_empty(taskqueue)) {
        struct future *f = list_entry(list_pop_func(taskqueue), struct future, elem);
        assert (f->state == FUTURE_NEW);
        f->state = FUTURE_INPROGRESS;
        f->executor = executor;
        if (f->owner == NULL)   // claim global submissions as owner
            f->owner = executor;
        havetask = true;
        *of = f;
    }

    return havetask;
}

static bool 
thread_pool_get_task(struct worker *executor,
                     struct list *taskqueue, 
                     pthread_mutex_t *queuelock, 
                     struct list_elem * (* list_pop_func)(struct list *), 
                     struct future **of)
{
    pthread_mutex_lock(queuelock);
    bool havetask = thread_pool_get_task_locked(executor, taskqueue, list_pop_func, of);
    pthread_mutex_unlock(queuelock);
    return havetask;
}

/**
 * Try to steal a task of minimum depth mindepth from victim.
 */
static bool 
try_to_steal_locked(struct worker *thief, struct worker * victim, struct future **of, int mindepth) 
{
    assert(thief != victim);

    struct list_elem *e;
    for (e = list_rbegin (&victim->queue); e != list_rend (&victim->queue); 
         e = list_prev (e)) 
    {
        struct future *q = list_entry(e, struct future, elem);
        // leapfrogging rule
        if (!q->depth > mindepth)
            continue;
       
        list_remove(e); 
        assert (q->state == FUTURE_NEW);

        q->state = FUTURE_INPROGRESS;
        q->executor = thief;
        *of = q;
        return true;
    }

    return false;
}

static bool 
try_to_steal(struct worker *thief, struct worker * victim, struct future **of, int mindepth) 
{
    pthread_mutex_lock(&victim->lock);
    bool havetask = try_to_steal_locked(thief, victim, of, mindepth);
    pthread_mutex_unlock(&victim->lock);
    return havetask;
}

#ifdef TRY_RANDOM_STEALING
/* xorshift64* http://xorshift.di.unimi.it/ */
static unsigned long
srand_r(unsigned long *seed) {
    unsigned long x = *seed;
    x ^= x >> 12; // a
    x ^= x << 25; // b
    x ^= x >> 27; // c
    *seed = x;
    return x * 2685821657736338717LL;
}
#endif

/**
 * Scan other worker queues for an opportunity to steal.
 */
static bool
scan_for_steals(struct worker *worker, int mindepth)
{
    struct thread_pool * pool = worker->pool;
    int tid, M = pool->nthreads;
#if TRY_RANDOM_STEALING
    int startat = srand_r(&worker->srand) % M;
#else
    int startat = 0;
#endif
    for (tid = startat; ; tid = (tid + 1) % M) {
        struct worker * pvictim = &pool->workers[tid];
        struct future * f;
        if (worker != pvictim && try_to_steal(worker, pvictim, &f, mindepth)) {
#if KEEP_STATS
            worker->taskstolen++;
#endif
            run_task(worker, f);
            return true;
        }

        if (tid == (startat - 1 + M) % M)
            break;
    }
    return false;
}

static void *
worker_loop(void *_worker)
{
    struct worker * worker = _worker;
    struct thread_pool *pool = worker->pool;
    current_worker = worker;
    struct future *f;
    install_stack_overflow_handler();

outer:
    while (true) {
        // process local queue first, in LIFO order
        while (thread_pool_get_task(worker, &worker->queue, &worker->lock, list_pop_front, &f))
            run_task(worker, f);

        // out of tasks on our local queue, let's check the global queue, in FIFO
        if (thread_pool_get_task(worker, &pool->submissionqueue, &pool->lock, list_pop_back, &f)) {
#if KEEP_STATS
            worker->globaltasks++;
#endif
            run_task(worker, f);
        }

        // try to steal, and if successful, check local queue & global queue again
        if (scan_for_steals(worker, 0))
            goto outer;

        // if a task is spawned now, we won't be able to steal it until 
        // we have reached the pthread_cond_wait() point and are woken up.
        // loss of efficiency only, may have to wait for next task spawned.

        // could not steal - blocking wait for work on global queue
        pthread_mutex_lock(&pool->lock);
        if (list_empty(&pool->submissionqueue) && !pool->shutdown)
            pthread_cond_wait(&pool->new_submissions, &pool->lock);

        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->lock);
            break;
        }

        bool have_task = thread_pool_get_task_locked(worker, &pool->submissionqueue, list_pop_back, &f);
        pthread_mutex_unlock(&pool->lock);

        if (have_task)
            run_task(worker, f);
    }
    return NULL;
}

/* Shutdown this executor service */
void 
thread_pool_shutdown_and_destroy(struct thread_pool *pool)
{
    int i;
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = true;
    pthread_cond_broadcast(&pool->new_submissions);
    pthread_mutex_unlock(&pool->lock);

    for (i = 0; i < pool->nthreads; i++) {
        pthread_join(pool->workers[i].thread, NULL);
    }
#if KEEP_STATS
    {
        int taskprocessed = 0, globaltasks = 0, taskstolen = 0, 
            taskleapfrogged = 0, futurereuse = 0, tasksubmitted = 0, 
            unabletohelp = 0, joininprogressfuture = 0;

        for (i = 0; i < pool->nthreads; i++) {
            struct worker * worker = pool->workers + i;
            taskprocessed += worker->taskprocessed;
            globaltasks += worker->globaltasks;
            taskstolen += worker->taskstolen;
            taskleapfrogged += worker->taskleapfrogged;
            futurereuse += worker->futurereuse;
            tasksubmitted += worker->tasksubmitted;
            unabletohelp += worker->unabletohelp;
            joininprogressfuture += worker->joininprogressfuture;
            fprintf(stderr, "Worker %2d: processed: %5d; global: %3d; stolen: %3d; leapfrogged: %3d; %5d/%5d reuse; nohelp: %4d/%4d.\n", 
                i, worker->taskprocessed, worker->globaltasks, 
                   worker->taskstolen, worker->taskleapfrogged, 
                   worker->futurereuse, worker->tasksubmitted,
                   worker->unabletohelp, worker->joininprogressfuture);
        }

        fprintf(stderr, "Total    : processed: %5d; global: %3d; stolen: %3d; leapfrogged: %3d; %5d/%5d reuse; nohelp: %4d/%4d.\n", 
            taskprocessed, globaltasks, 
            taskstolen, taskleapfrogged, 
            futurereuse, tasksubmitted,
            unabletohelp, joininprogressfuture);
    }
#endif
    free(pool->workers);
    free(pool);
}

static void * 
future_wait_blocking(struct future *f) {
    pthread_mutex_lock(&f->lock);
    while (!f->isdone)
        pthread_cond_wait(&f->statechange, &f->lock);

    void * result = f->result;
    pthread_mutex_unlock(&f->lock);
    return result;
}

/* Wait until future task has finished executing (if necessary);
 * retrieve and return result.  
 */
void * 
future_get(struct future *q)
{
    struct worker * worker = current_worker;
    if (worker == NULL)
        return future_wait_blocking(q);

    // else we are internal
recheck:
    pthread_mutex_lock(&q->owner->lock);
    if (q->state == FUTURE_DONE) {  // a race since it is set without owner lock
                                    // in run_task.
        pthread_mutex_unlock(&q->owner->lock);
        return q->result;           // ditto
    }

    // if the future is still new, grab it
    if (q->state == FUTURE_NEW) {
        list_remove(&q->elem);
        q->state = FUTURE_INPROGRESS;
        q->executor = worker;
        q->depth = max(q->depth, worker->current_depth + 1);

        pthread_mutex_unlock(&q->owner->lock);
        // ----------------------------------------------
        return run_task(worker, q);
    }

    // helping protocol
    while (q->state == FUTURE_INPROGRESS) {
#if KEEP_STATS
        worker->joininprogressfuture++;
#endif
        // Task is being worked on.   Try to leapfrog
        // see if we can steal from executor
        struct worker *victim = q->executor;
        assert (worker != victim);

        struct future *f;
        int mindepth = max(q->depth, worker->current_depth);

        pthread_mutex_unlock(&q->owner->lock);

        if (try_to_steal(worker, victim, &f, mindepth)) {
#if KEEP_STATS
            worker->taskstolen++;
            worker->taskleapfrogged++;
#endif
            run_task(worker, f);
            goto recheck;
        }

        // could not steal from victim's queue - check if I can do something else
        if (scan_for_steals(worker, mindepth))
            goto recheck;

        // I'm here because I could not find any suitable task to steal.
        // So break and block if task is still not done.
        break;
    }

#if KEEP_STATS
    worker->unabletohelp++;
#endif
    return future_wait_blocking(q);
}

/* Deallocate this future. */
void 
future_free(struct future *f)
{
    struct worker * worker = current_worker;
    if (worker == NULL || worker->nfree_futures > MAX_FUTURE_FREE_LIST_LENGTH) {
        free(f);
    } else {
        list_push_back(&worker->free_futures, &f->elem);
        worker->nfree_futures++;
    }
}
