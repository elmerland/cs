#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <semaphore.h>

double sum;
long thread_count;
long long n;
pthread_mutex_t mutex;
sem_t semaphore;

void * Thread_mutex_sum(void * rank) {
    long my_rank = (long) rank;
    double factor;
    double my_sum = 0.0;
    long long i;
    long long my_n = n / thread_count;
    long long my_first_i = my_n * my_rank;
    long long my_last_i = my_first_i + my_n;
    
    if (my_first_i % 2 == 0) {
        factor = 1.0;
    } else {
        factor = -1.0;
    }
    
    for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
        my_sum += factor / (2 * i + 1);
    }
    
    pthread_mutex_lock(&mutex);
    sum += my_sum;
    pthread_mutex_unlock(&mutex);

    // for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
    //     pthread_mutex_lock(&mutex);
    //     sum += factor / (2 * i + 1);
    //     pthread_mutex_unlock(&mutex);
    // }

    return NULL;
} /* Thread_sum */


void * Thread_sem_sum(void * rank) {
    long my_rank = (long) rank;
    double factor;
    double my_sum = 0.0;
    long long i;
    long long my_n = n / thread_count;
    long long my_first_i = my_n * my_rank;
    long long my_last_i = my_first_i + my_n;
    
    if (my_first_i % 2 == 0) {
        factor = 1.0;
    } else {
        factor = -1.0;
    }
    
    for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
        my_sum += factor / (2 * i + 1);
    }
    
    sem_wait(&semaphore);
    sum += my_sum;
    sem_post(&semaphore);

    // for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
    //     sem_wait(&semaphore);
    //     sum += factor / (2 * i + 1);
    //     sem_post(&semaphore);
    // }
    
    return NULL;
} /* Thread_sum */

int main(int argc, char ** argv) {
    if (argc != 3) {
        printf("Invalid parameters\n");
        exit(1);
    }

    int mode = strtol(argv[1], NULL, 10);
    int power = strtol(argv[2], NULL, 10);
    n = (long long) pow(2.0, (double)power);
    sum = 0.0;
    thread_count = 16;
    long i, err;
    struct timeval start, end;
    pthread_t * t_ids = 
        (pthread_t *) malloc(sizeof(pthread_t) * (thread_count - 1));
    
    if (mode == 1) {
        pthread_mutex_init(&mutex, NULL);
    } else {
        sem_init(&semaphore, 0, 1);
    }
    

    gettimeofday(&start, NULL);

    for (i = 0; i < thread_count - 1; i++) {
        if (mode == 1) {
            err = pthread_create(&t_ids[i], NULL, Thread_mutex_sum, (void *)i);
        } else {
            err = pthread_create(&t_ids[i], NULL, Thread_sem_sum, (void *)i);
        }
        
        if (err) {
            perror("Could not start thread");
            exit(1);
        }
    }

    if (mode == 1) {
        Thread_mutex_sum((void *) (thread_count - 1));
    } else {
        Thread_sem_sum((void *) (thread_count - 1));
    }
    

    for (i = 0; i < thread_count - 1; i++) {
        err = pthread_join(t_ids[i], NULL);
        if (err) {
            perror("Could not join thread");
            exit(1);
        }
    }

    gettimeofday(&end, NULL);
    printf("%s thread: %2ld n: %lld est: %2.10f time: %ld\n", mode == 1 ? "Mutex" : "Semaphore"
        , thread_count, n, sum * 4,
        ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));

    return 0;
}

