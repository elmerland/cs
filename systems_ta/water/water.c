
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define ROUNDS 100
#define N 30

void * oxygen_thread(void * args);
void * hydrogen_thread(void * args);
void bond(void);

pthread_mutex_t mutex;
int oxygen = 0;
int hydrogen = 0;
int bonds = 0;
pthread_barrier_t barrier;
sem_t oxyQueue;
sem_t hydroQueue;

int main(int argc, char ** argv) {
  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, 3);
  sem_init(&oxyQueue, 0, 0);
  sem_init(&hydroQueue, 0, 0);
  int k, i;
  for (k = 0; k < ROUNDS; k++) {
    pthread_t tid[N];
    for (i = 0; i < N; i++) {
      if (i % 3 == 0) { pthread_create(&tid[i], NULL, &oxygen_thread  , NULL); }
      else            { pthread_create(&tid[i], NULL, &hydrogen_thread, NULL); }
    }
    for (i = 0; i < N; i++) {
      pthread_join(tid[i], NULL);
    }
    // printf("%d H2O\n", bonds);
    printf("- %d%s", bonds, k%3 == 0 ? "\n" : "\t");
  }
  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&barrier);
  sem_destroy(&oxyQueue);
  sem_destroy(&hydroQueue);
  return 0;
}

void * oxygen_thread(void * args) {
  bool locked = false;
  pthread_mutex_lock(&mutex);
  locked = true;
  oxygen += 1;
  if (hydrogen >= 2) {
    sem_post(&hydroQueue);
    sem_post(&hydroQueue);
    hydrogen -= 2;
    sem_post(&oxyQueue);
    oxygen -= 1;

    
  } else {
    pthread_mutex_unlock(&mutex);
    locked = false;
  }

  sem_wait(&oxyQueue);
  bond();
  pthread_barrier_wait(&barrier);

  if (locked) pthread_mutex_unlock(&mutex);

  return NULL;
}

void * hydrogen_thread(void * args) {
  pthread_mutex_lock(&mutex);
  hydrogen += 1;
  if (hydrogen >= 2 && oxygen >= 1) {
    sem_post(&hydroQueue);
    sem_post(&hydroQueue);
    hydrogen -= 2;
    sem_post(&oxyQueue);
    oxygen -= 1;

    sem_wait(&hydroQueue);
    pthread_barrier_wait(&barrier);
    pthread_mutex_unlock(&mutex);
  } else {
    pthread_mutex_unlock(&mutex);
    sem_wait(&hydroQueue);
    pthread_barrier_wait(&barrier);
  }
  return NULL;
}

void bond(void) {
  // printf("A H2O molecule is formed\n");
  printf("# ");
  bonds++;
}
