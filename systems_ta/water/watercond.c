/**
 * Solution to 'water' exercise - translate the solution given in the
 * little book of semaphores into POSIX Threads.
 *
 * This version uses condition variables instead of mutexes
 *
 * CS 3214 Fall 2014
 * @author gback@cs.vt.edu
 */
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

static pthread_mutex_t mutex;
static pthread_cond_t hydroQueue;
static pthread_cond_t oxyQueue;
static int oxygen;
static int hydrogen;
static pthread_barrier_t barrier;
static volatile int bonds;

static void *
oxygen_thread(void *_)
{
    pthread_mutex_lock(&mutex);
    oxygen++;
    while (hydrogen < 2) {
        pthread_cond_wait(&oxyQueue, &mutex);
    }

    pthread_cond_signal(&hydroQueue);
    pthread_cond_signal(&hydroQueue);

    hydrogen -= 2;
    oxygen -= 1;

    printf("A H2O molecule is formed\n");
    bonds++;        // incrementing bonds will allow hydrogen to move on

    pthread_mutex_unlock(&mutex);
    pthread_barrier_wait(&barrier);
}

static void *
hydrogen_thread(void *_)
{
    pthread_mutex_lock(&mutex);
    hydrogen++;
    int bonds_on_entry = bonds;
    // wait, while either there's no other hydrogen or no oxygen,
    // or until after oxygen thread did bond us (bonds is no longer
    // the same as bonds on entry)
    while (bonds_on_entry == bonds && (hydrogen < 2 || oxygen < 1))
        pthread_cond_wait(&hydroQueue, &mutex);

    pthread_cond_signal(&oxyQueue);

    pthread_mutex_unlock(&mutex);
    pthread_barrier_wait(&barrier);
}

int
main()
{
    pthread_cond_init(&hydroQueue, NULL);
    pthread_cond_init(&oxyQueue, NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, 3);

#define ROUNDS 100
    int k;
    for (k = 0; k < ROUNDS; k++) {
#define N 30
        pthread_t t[N];
        int i;
        for (i = 0; i < N; i++) {
            if (i % 3 == 0)
                pthread_create(t + i, NULL, oxygen_thread, NULL);
            else
                pthread_create(t + i, NULL, hydrogen_thread, NULL);
        }

        for (i = 0; i < N; i++)
            pthread_join(t[i], NULL);
        printf("%d H20\n", bonds);
    }
    return 0;
}
