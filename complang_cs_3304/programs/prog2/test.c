#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

void * timer(void * rank);
void * sync(void * rank);

int       thread_count;
int       mode;
pthread_t * t_ids;
sem_t * semaphores;

int main(int argc, char ** argv) {
  if (argc != 3) {
    puts("Invalid arguments");
    exit(1);
  }

  thread_count = strtol(argv[1], NULL, 10);
  mode         = strtol(argv[2], NULL, 10);

  t_ids = (pthread_t *) malloc(thread_count * sizeof(pthread_t));
  semaphores = (sem_t *) malloc(thread_count * sizeof(sem_t));

  long i;

  struct timeval start, end;
  if (mode == 1) {
    printf("Stat timer");
    gettimeofday(&start, NULL);
  }

  for (i = 0; i < thread_count; i++) {
    if (mode == 1) {
      pthread_create(&t_ids[i], NULL, timer, (void *) i);
    } else if (mode == 2) {
      sem_init(&semaphores[i], 0, 0);
      pthread_create(&t_ids[i], NULL, sync, (void *) i);
    }
  }

  if (mode == 2) {
    sem_post(&semaphores[0]);
  }

  for (i = 0; i < thread_count; i++) {
    pthread_join(t_ids[i], NULL);
  }

  if (mode == 1) {
    gettimeofday(&end, NULL);
    long t = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
    printf("Timer: %ld\n", t);
  }

  free(t_ids);
  return 0;
}

void * timer(void * rank) {
  long my_rank = (long)rank;
  char buf[256];
  sprintf(buf, "Hello from thread: %ld", my_rank);
  puts(buf);
  return NULL;
}

void * sync(void * rank) {
  long my_rank = (long) rank;
  sem_wait(&semaphores[my_rank]);
  printf("Hello from thread: %ld\n", my_rank);
  if (my_rank < (thread_count - 1)) {
    sem_post(&semaphores[my_rank + 1]);
  }
  return NULL;
}
