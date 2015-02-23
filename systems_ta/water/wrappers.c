
#include "wrappers.h"

#include <stdlib.h>
#include <stdio.h>

int Pthread_mutex_destroy(pthread_mutex_t * mutex) {
  int error = Pthread_mutex_destroy(mutex);
  if (error != 0) {
    printf("Error destroying pthread mutex\n");
    exit(1);
  }
  return error;
}

int Pthread_mutex_init(pthread_mutex_t * mutex, const pthread_mutexattr_t * attr) {
  int error = pthread_mutex_init(mutex, attr);
  if (error != 0) {
    printf("Error initializing pthread mutex\n");
    exit(1);
  }
  return error;
}

int Pthread_barrier_wait(pthread_barrier_t *barrier) {

}

int Pthread_barrier_destroy(pthread_barrier_t *barrier) {

}

int Pthread_barrier_init(pthread_barrier_t * barrier, const pthread_barrierattr_t * attr, unsigned count) {
  
}
