#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include "threadpool.h"
#include "list.h"

struct thread_pool {
	// List of future structures.
	struct list future_list;
	// Pointer for array of threads.
	pthread_t * thread_list;
	// Number of threads to be created.
	int nthreads;
	// Flag to determine if thread execution should stop.
	bool done;
	// Mutex lock for access to future list.
	pthread_mutex_t future_lock;
	// Conditional variable to determine if there are any futures available.
	pthread_cond_t  avail_item;
};

struct future {
	// Function to be called by the thread.
	thread_pool_callable_func_t callable;
	// Parameter to be given to the callable function.
	void * callable_param;
	// Result of calling the function.
	void * result;
	// Semaphore for this future's result.
	sem_t result_sem;
	// List element to allow this structure to be added to a list.
	struct list_elem elem;
};

static void * future_consumer(void * arg) {
	struct thread_pool * tp = (struct thread_pool *)arg;
	// Start processing futures
	while (true) {
		// Get lock on future list.
		pthread_mutex_lock(&tp->future_lock);
		// Check if future list is empty.
		while (list_empty(&tp->future_list) && !tp->done) {
			// If list is empty wait for item available signal.
			pthread_cond_wait(&tp->avail_item, &tp->future_lock);
		}
		// Check if thread pool is done.
		if (tp->done) {
			pthread_mutex_unlock(&tp->future_lock);
			return NULL;
		}
		// Remove future from front of list.
		struct list_elem * f_elem = list_pop_front(&tp->future_list);
		// Release lock on future list.
		pthread_mutex_unlock(&tp->future_lock);
		// Get future struct from list element
		struct future * f_struct = list_entry(f_elem, struct future, elem);
		// Call function of future, pass in parameter, and get result.
		f_struct->result = (*f_struct->callable)(f_struct->callable_param);
		// Unlock semaphore
		sem_post(&f_struct->result_sem);
	}
	return NULL;
}

/* Create a new thread pool with n threads. */
struct thread_pool * thread_pool_new(int nthreads) {
	// Initialize the thread pool structure.
	struct thread_pool * tp = malloc(sizeof(struct thread_pool));
	// Initialize future list inside of thread pool.
	list_init(&tp->future_list);
	// Set done flag to false.
	tp->done = false;
	// Initialize lock.
	pthread_mutex_init(&tp->future_lock, NULL);
	// Initialize item available conditional variable.
	pthread_cond_init (&tp->avail_item, NULL);
	// Allocate space for all the pthreads of the thread pool.
	tp->thread_list = calloc(nthreads, sizeof(pthread_t));
	tp->nthreads = nthreads;

	// Initialize and run threads.
	int i;
	for (i = 0; i < nthreads; i++) {
		pthread_create(&tp->thread_list[i], NULL, future_consumer, tp);
	}
	return tp;
}

/* Shutdown this thread pool.  May or may not execute already queued tasks. */
void thread_pool_shutdown(struct thread_pool * tp) {
	pthread_mutex_lock(&tp->future_lock);
	// Set done flag to true.
	tp->done = true;
	// Signal all waiting threads to avoid deadlock.
	pthread_cond_broadcast(&tp->avail_item);
	pthread_mutex_unlock(&tp->future_lock);

	// Reap all threads
	int i;
	for (i = 0; i < tp->nthreads; i++) {
		pthread_join(tp->thread_list[i], NULL);
	}
	// Deallocate thread array.
	free(tp->thread_list);
}

/* Submit a callable to thread pool and return future.
 * The returned future can be used in future_get() and future_free()
 */
struct future * thread_pool_submit(
        struct thread_pool * tp,
        thread_pool_callable_func_t callable_func,
        void * callable_data) {
	// Initialize future structure
	struct future * f = malloc(sizeof(struct future));
	// Set callable function.
	f->callable = callable_func;
	// Set function parameter.
	f->callable_param = callable_data;
	// Set result to NULL.
	f->result = NULL;
	// Initialize semaphore
	sem_init(&f->result_sem, 0, 1);
	// Decrease semaphore to lock it until the result is available.
	sem_wait(&f->result_sem);

	pthread_mutex_lock(&tp->future_lock);
	// Add future to list.
	list_push_back(&tp->future_list, &f->elem);
	// Signal item available.
	pthread_cond_signal(&tp->avail_item);
	pthread_mutex_unlock(&tp->future_lock);
	return f;
}

/* Make sure that thread pool has completed executing this callable,
 * then return result. */
void * future_get(struct future * f) {
	sem_wait(&f->result_sem);
	sem_destroy(&f->result_sem);
	return f->result;
}

/* Deallocate this future.  Must be called after future_get() */
void future_free(struct future * f) {
	free(f);
}

