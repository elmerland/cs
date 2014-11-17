#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <omp.h>

void  print_usage     (char * str);
void  get_params      (int argc, char ** argv);
int   which_bin       (float data);
void  init_locks      ();
void  destroy_locks   ();
void  process_data    (void);
float get_rand        (float min, float max);
float gen_data        ();
int   get_count       (int t_id);
void  print_histogram ();

int     bin_count_p;
float   min_meas_p;
float   max_meas_p;
int     data_count_p;
int     thread_count_p;
float   bin_width;
int   * bins;
omp_lock_t * bin_locks;

#define DEBUG

int main(int argc, char ** argv) {
  get_params(argc, argv); // Get parameters

  // Allocate data
  bins      = (int *)        calloc(bin_count_p, sizeof(int));
  bin_locks = (omp_lock_t *) calloc(bin_count_p, sizeof(omp_lock_t));

  // Wind up threads
  srand(time(NULL));
  init_locks();

  process_data();

  destroy_locks();

  // Print result
  print_histogram();
  
  // Free allocated data
  free(bins);
  free(bin_locks);
  return 0;
}

/**
 * Print usage
 * @param str Name of the program
 */
void print_usage(char * str) {
  fprintf(stderr, "Usage: %s ", str);
  fprintf(stderr, "<bin_count> <min_meas> <max_meas> <data_count> <thread_count>\n");
  exit(0);
}

/**
 * Get the parameters from user input.
 * @param argc The parameter count
 * @param argv The parameter values
 */
void get_params(int argc, char ** argv) {
  if (argc != 6) {
    print_usage(argv[0]);
  }

  bin_count_p    = strtol(argv[1], NULL, 10);
  min_meas_p     = strtof(argv[2], NULL);
  max_meas_p     = strtof(argv[3], NULL);
  data_count_p   = strtol(argv[4], NULL, 10);
  thread_count_p = strtol(argv[5], NULL, 10);
  bin_width      = (max_meas_p - min_meas_p) / bin_count_p;

  #ifdef DEBUG
  printf("bins: %5d\t"              , bin_count_p);
  printf("min: %5.2f\tmax: %5.2f\t" , min_meas_p   ,max_meas_p);
  printf("data: %5d\tthreads: %2d\t", data_count_p ,thread_count_p);
  printf("bin_width: %f\n", bin_width);
  #endif
}

/**
 * Determines in which bin index the given data point falls in.
 * @param  data The data point
 * @return      The bin index where the data point belongs
 */
int which_bin(float data) {
  int bin_index = (int) ((data - min_meas_p) / bin_width);
  int adjusted_bin_index;

  if (bin_index >= bin_count_p) {
    adjusted_bin_index = bin_count_p;
  } else if (bin_index < 0) {
    adjusted_bin_index = 0;
  } else {
    adjusted_bin_index = bin_index;
  }

  #ifdef DEBUG
  printf("d: %4.3f\tb: %2d\ta_b:%2d %s\n",
    data, bin_index, adjusted_bin_index,
    bin_index != adjusted_bin_index ? "ADJUSTED":"");
  #endif

  return adjusted_bin_index;
}

/**
 * Initialize all the mutex locks needed for this program
 */
void init_locks() {
  int i;
  for (i = 0; i < bin_count_p; i++) {
    omp_init_lock(&bin_locks[i]);
  }
}

/**
 * Destroy all initialized locks
 */
void destroy_locks() {
  int i;
  for (i = 0; i < bin_count_p; i++) {
    omp_destroy_lock(&bin_locks[i]);
  }
}

/**
 * Funtion for worker threads to execute. This function creates the thread's
 * share of random data, stores it in a local bin list and finally adds up all
 * bin counts to the master bin list.
 * @param  args NULL
 * @return      NULL
 */
void process_data(void) {
  omp_set_dynamic(0);
  if (thread_count_p == 0) {
    thread_count_p = omp_get_num_procs();
  }
  omp_set_num_threads(thread_count_p);

  #pragma omp parallel
  {
    int     rank       = omp_get_thread_num();
    int     count      = get_count(rank);
    float * bins_local = (float *)calloc(bin_count_p, sizeof(float));

    #ifdef DEBUG
      #pragma omp critical
      {
        char buff[256];
        sprintf(buff, "Thread - Rank %2d Count %4d", rank, count);
        puts(buff);
      }
      #pragma omp barrier
      #pragma omp master
      {
        printf("Total threads: %d\n", omp_get_num_threads());
      }
      #pragma omp barrier
    #endif

    // Generate data and add to bins
    int i;
    for (i = 0; i < count; i++) {
      bins_local[which_bin(gen_data())]++;
    }

    // Merge local bins to master bins
    for (i = 0; i < bin_count_p; i++) {
      omp_set_lock(&bin_locks[i]);
      bins[i] += bins_local[i];
      omp_unset_lock(&bin_locks[i]);
    }

    free(bins_local);
  }
}

/**
 * Get a random value within the specified range
 * @param  min The minimum value
 * @param  max The maximum value
 * @return     A random value in the specified range
 */
float get_rand(float min, float max) {
  return (max - min) * (float)((double)rand() / (double)RAND_MAX) + min;
}

/**
 * Get a single data point
 * @return The data point
 */
float gen_data() {
  return get_rand(min_meas_p, max_meas_p);
}

/**
 * The number of data points that thread at rank 't_id' needs to generate.
 * @param  t_id Thread rank
 * @return      Count for thread
 */
int get_count(int t_id) {
  int thread_num = omp_get_num_threads();
  int r = data_count_p % thread_num;
  int offset, step, idx;
  if (t_id < r) {
    offset = 0;
    step   = (data_count_p / thread_num) + 1;
    idx    = t_id;
  } else {
    offset = ((data_count_p / thread_num) + 1) * r;
    step   = data_count_p / thread_num;
    idx    = t_id - r;
  }

  int start = offset + (step * idx);
  int end   = (start) + step;

  return end - start;
}

/**
 * Prints a visual representation of the historgram data.
 */
void print_histogram() {
  int   i, j, total = 0;
  float min, max;
  for (i = 0; i < bin_count_p; i++) {
    min = (bin_width * i) + min_meas_p;
    max = min + bin_width;
    printf("[%2d] %4.3f - %4.3f:\t", i, min, max);
    for (j = 0; j < bins[i]; j++) {
      printf("X");
      total++;
    }
    printf("\n");
  }
  if (total != data_count_p) {
    printf("Error: Data count does not match %d != %d", total, data_count_p);
  }
}
