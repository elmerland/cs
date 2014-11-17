#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <semaphore.h>

void   pthread_barrier    ();
void   init_semaphores    ();
void   lock_semaphores    ();
void   destroy_semaphores ();
void   create_threads     ();
void   join_threads       ();
void   read_x             ();
void * Pth_mat_vect       (void * rank);
void   get_offset         (int rank, int threads, long rows, long * start, long * end);

int               thread_count;
pthread_t       * t_ids;
sem_t           * semaphores;
long              m, n;
double          * x;
pthread_mutex_t   mutex;
pthread_cond_t    cond_var;
int               counter = 0;

int main(int argc, char ** argv) {
  if (argc != 2) {
    printf("Invalid arguments\n");
    exit(1);
  }
  // Read m and n values
  scanf("%ld %ld", &m, &n);

  struct timeval start, end;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_var, NULL);
  thread_count = strtol(argv[1], NULL, 10);
  t_ids        = (pthread_t *) malloc(sizeof(pthread_t) * thread_count);
  semaphores   = (sem_t *) malloc(sizeof(sem_t) * thread_count);
  x            = (double *) malloc(sizeof(double) * n);
  
  read_x();
  init_semaphores();
  lock_semaphores();
  sem_post(&semaphores[0]); // Start the first thread
  create_threads();

  pthread_barrier();
  gettimeofday(&start, NULL);
  
  pthread_barrier();
  gettimeofday(&end, NULL);
  long t = (end.tv_sec * 1000000 + end.tv_usec) -
           (start.tv_sec * 1000000 + start.tv_usec);

  join_threads();
  destroy_semaphores();

  printf("t: %d m: %ld n: %ld time: %ld\n", thread_count, m, n, t);

  free(x);
  free(t_ids);
  free(semaphores);
  return 0;
}

void pthread_barrier() {
  pthread_mutex_lock(&mutex);
  counter++;
  if (counter == (thread_count + 1)) {
    counter = 0;
    pthread_cond_broadcast(&cond_var);
  } else {
    while (pthread_cond_wait(&cond_var, &mutex) != 0);
  }
  pthread_mutex_unlock(&mutex);
}

void init_semaphores() {
  int i;
  for (i = 0; i < thread_count; i++) {
    sem_init(&semaphores[i], 0, 1);
  }
}

void lock_semaphores() {
  int i;
  for (i = 0; i < thread_count; i++) {
    sem_wait(&semaphores[i]);
  }
}

void destroy_semaphores() {
  int i;
  for (i = 0; i < thread_count; i++) {
    sem_destroy(&semaphores[i]);
  }
}

void create_threads() {
  long i, err;

  for (i = 0; i < thread_count; i++) {
    err = pthread_create(&t_ids[i], NULL, Pth_mat_vect, (void *) i);
    if (err) {
      perror("Could not create thread");
      exit(1);
    }
  }
}

void join_threads() {
  int i, err;
  for (i = 0; i < thread_count; i++) {
    err = pthread_join(t_ids[i], NULL);
    if (err) {
      perror("Could not join thread");
      exit(1);
    }
  }
}

void read_x() {
  int i;
  for (i = 0; i < n; i++) {
    scanf("%lf", &x[i]);
  }
}

void * Pth_mat_vect(void * rank) {
  long my_rank = (long) rank;
  int i, j;
  long my_first_row, my_last_row;
  get_offset(my_rank, thread_count, m, &my_first_row, &my_last_row);

  // Initialize A array
  long num_rows = my_last_row - my_first_row;
  long arr_sz = num_rows * n;
  double * A = (double *) calloc(arr_sz, sizeof(double));
  double * y = (double *) calloc(num_rows, sizeof(double));

  // Read in array
  sem_wait(&semaphores[my_rank]);
  for (i = 0; i < arr_sz; i++) {
    scanf("%lf", &A[i]);
  }
  if (my_rank < thread_count - 1) {
    sem_post(&semaphores[my_rank + 1]);
  } else {
    sem_post(&semaphores[0]);
  }

  pthread_barrier();

  // Calculate y value
  for (i = 0; i < num_rows; i++) {
    y[i] = 0.0;
    for (j = 0; j < n; j++) {
      y[i] += A[i * n + j] * x[j];
    }
  }

  pthread_barrier();

  // Output results
  sem_wait(&semaphores[my_rank]);
  for (i = 0; i < num_rows; i++) {
    printf("y[%2ld] %lf\n", i + my_first_row, y[i]);
  }
  if (my_rank < thread_count - 1) {
    sem_post(&semaphores[my_rank + 1]);
  }
  free(y);
  free(A);
  return NULL;
} /* Pth_mat_vect */

void get_offset( int rank, int threads, long rows, long * start, long * end) {
  long r = rows % threads;
  long offset, step, idx;
  if (rank < r) {
    offset = 0;
    step = (rows / threads) + 1;
    idx = rank;
  } else {
    offset = ((rows / threads) + 1) * r;
    step = rows / threads;
    idx = rank - r;
  }
  *start = offset + (step * idx);
  *end = (*start) + step;
} /* get_offset */
