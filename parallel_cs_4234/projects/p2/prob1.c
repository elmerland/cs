#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define lint unsigned long long int

/**
 * Runs toss simulation and stores in circle count in array.
 * @param  arg NULL
 * @return     NULL
 */
void * t_func(void * arg);

/**
 * Gets the rank value and increases rank by one.
 * @return The current rank before increase.
 */
int get_rank();

/**
 * Get a random number between min and max
 * @param  min The min
 * @param  max The max
 * @return     A random number
 */
double get_rand(double min, double max);

/**
 * Gets the number of tosses for each thread to perform
 * @param  p_id      The rank of this thread
 * @param  processes The number of processes
 * @param  values    The number of tosses
 * @return           The number of tosses for thread with rank p_id
 */
lint get_count(int p_id, int processes, lint values);

// The total number of thread to be created (including the maste thread)
int thread_count;
// Array of counts for each thread.
lint * global_in_circle_counts;
// Total number of tosses to be performed by all threads
lint total_toss_count;
// The rank counter to determine rank of each thread.
int rank_count = 0;
// Lock for rank count
pthread_mutex_t rank_lock;

int main(int argc, char ** argv) {
  // Check argument length
  if (argc != 3) {
    printf("Usage: ./prob1 <thread_count> <toss_count>\n");
    exit(0);
  }

  // Initialize
  int i, err;
  srand(time(NULL));
  err = pthread_mutex_init(&rank_lock, NULL);
  if (err) {
    perror("Could no initialize rank mutex");
    exit(1);
  }

  // Get the thread count
  thread_count = atoi(argv[1]);
  pthread_t t_ids[thread_count - 1]; // Subtract one because of main thread

  // Get the total toss count and create toss count array
  total_toss_count        = (lint)atoi(argv[2]);
  global_in_circle_counts = (lint *)malloc(sizeof(lint) * thread_count);

  printf("Start: %3d threads and %10llu tosses - ",
    thread_count, total_toss_count);

  // Created threads
  for (i = 0; i < thread_count; i++) {
    err = pthread_create(&t_ids[i], NULL, &t_func, (void *) &total_toss_count);
    if (err) {
      perror("Could not create thread.");
      exit(1);
    }
  }

  // Do work in main thread
  t_func((void *) &total_toss_count);

  // Reap threads
  for (i = 0; i < thread_count; i++) {
    err = pthread_join(t_ids[i], NULL);
    if (err) {
      perror("Could not join thread");
      exit(1);
    }
  }

  // Add up all the in circle counts for each thread.
  lint total_in_circle_count = 0;
  for (i = 0; i < thread_count; i++) {
    total_in_circle_count += global_in_circle_counts[i];
  }

  // Estimate PI and print
  double pi_estimate = 4 * total_in_circle_count / (double) total_toss_count;
  printf("PI: %2.15f\n", pi_estimate);

  // Free structures
  free(global_in_circle_counts);
  return 0;
}

void * t_func(void * arg) {
  int rank = get_rank();
  lint local_toss_count = get_count(rank, thread_count, total_toss_count);
  lint local_in_cirle_counts = 0;
  int i;
  for (i = 0; i < local_toss_count; i++) {
    double x    = get_rand(-1, 1);
    double y    = get_rand(-1, 1);
    double dist = x * x + y * y;
    if (dist <= 1) {
      local_in_cirle_counts++;
    }
  }
  global_in_circle_counts[rank] = local_in_cirle_counts;
  return NULL;
}

double get_rand(double min, double max) {
  return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

int get_rank() {
  pthread_mutex_lock(&rank_lock);
  int rank = rank_count;
  rank_count++;
  pthread_mutex_unlock(&rank_lock);
  return rank;
}

lint get_count(int p_id, int processes, lint values) {
   int r = values % processes;
   int offset, step, idx;
   if (p_id < r) {
      offset = 0;
      step   = (values / processes) + 1;
      idx    = p_id;
   } else {
      offset = ((values / processes) + 1) * r;
      step   = values / processes;
      idx    = p_id - r;
   }

   lint start = offset + (step * idx);
   lint end   = (start) + step;

   return end - start;
}
