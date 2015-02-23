
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
pthread_cond_t oxyQueue;
pthread_cond_t hydroQueue;

int main(int argc, char ** argv) {
  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, 3);
  pthread_cond_init(&oxyQueue, NULL);
  pthread_cond_init(&hydroQueue, NULL);
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
  pthread_cond_destroy(&oxyQueue);
  pthread_cond_destroy(&hydroQueue);
  return 0;
}

void * oxygen_thread(void * args) {
  pthread_mutex_lock(&mutex);
  oxygen += 1;
  if (hydrogen >= 2) {
    pthread_cond_signal(&hydroQueue);
    pthread_cond_signal(&hydroQueue);
    hydrogen -= 2;
    pthread_cond_signal(&oxyQueue);
    oxygen -= 1;

    pthread_cond_wait(&oxyQueue, &mutex);
    bond();
    pthread_barrier_wait(&barrier);
    pthread_mutex_unlock(&mutex);
  } else {
    pthread_cond_wait(&oxyQueue, &mutex);
    pthread_mutex_unlock(&mutex);
    bond();
    pthread_barrier_wait(&barrier);
  }
  return NULL;
}

void * hydrogen_thread(void * args) {
  pthread_mutex_lock(&mutex);
  hydrogen += 1;
  if (hydrogen >= 2 && oxygen >= 1) {
    pthread_cond_signal(&hydroQueue);
    pthread_cond_signal(&hydroQueue);
    hydrogen -= 2;
    pthread_cond_signal(&oxyQueue);
    oxygen -= 1;

    pthread_cond_wait(&hydroQueue, &mutex);
    pthread_barrier_wait(&barrier);
    pthread_mutex_unlock(&mutex);
  } else {
    pthread_cond_wait(&hydroQueue, &mutex);
    pthread_mutex_unlock(&mutex);
    pthread_barrier_wait(&barrier);
  }
  return NULL;
}

void bond(void) {
  // printf("A H2O molecule is formed\n");
  printf("# ");
  bonds++;
}
