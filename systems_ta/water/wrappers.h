#ifndef __WRAPPERS_H
#define __WRAPPERS_H

#include <pthread.h>

int Pthread_mutex_destroy(pthread_mutex_t *mutex);

int Pthread_mutex_init(pthread_mutex_t * mutex, const pthread_mutexattr_t * attr);

int Pthread_barrier_wait(pthread_barrier_t *barrier);

int Pthread_barrier_destroy(pthread_barrier_t *barrier);

int Pthread_barrier_init(pthread_barrier_t * barrier, const pthread_barrierattr_t * attr, unsigned count);

#endif
