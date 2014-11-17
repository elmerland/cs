#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <semaphore.h>

void   pthread_barrier    ();
void   create_threads     ();
void   join_threads       ();
void   read_x             ();
void   read_a             ();
void   print_y            ();
void * Pth_mat_vect       (void * rank);
void   get_offset         (int rank, int threads, long rows, long * start, long * end);

int               thread_count;
pthread_t       * t_ids;
sem_t           * semaphores;
long              m, n;
double          * x, * A, * y;
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
  x            = (double *) malloc(sizeof(double) * n);
  A            = (double *) malloc(sizeof(double) * (n * m));
  y            = (double *) malloc(sizeof(double) * m);
  
  read_x();
  read_a();

  create_threads();

  pthread_barrier();
  gettimeofday(&start, NULL);
  
  pthread_barrier();
  gettimeofday(&end, NULL);
  long t = (end.tv_sec * 1000000 + end.tv_usec) -
           (start.tv_sec * 1000000 + start.tv_usec);

  join_threads();

  // print_y();
  printf("t: %d m: %ld n: %ld time: %ld\n", thread_count, m, n, t);

  free(x);
  free(t_ids);
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

void read_a() {
  int i;
  for (i = 0; i < (n * m); i++) {
    scanf("%lf", &A[i]);
  }
}

void print_y() {
  long i;
  for (i = 0; i < m; i++) {
    printf("y[%ld] %lf\n", i, y[i]);
  }
}

void * Pth_mat_vect(void * rank) {
  long my_rank = (long) rank;
  int i, j;
  long my_first_row, my_last_row;
  get_offset(my_rank, thread_count, m, &my_first_row, &my_last_row);
  pthread_barrier();

  // Calculate y value
  for (i = my_first_row; i < my_last_row; i++) {
    y[i] = 0.0;
    for (j = 0; j < n; j++) {
      y[i] += A[i * n + j] * x[j];
    }
  }

  pthread_barrier();
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
