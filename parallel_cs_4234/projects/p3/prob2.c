#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

void get_args (int argc, char ** argv);
void do_work  ();

 int    maxwork; // Maximum amount of work to perform
 int    consumers; // Number of consumer threads
 int    producers; // Number of producer threads
char ** input_files; // File names for input files

#define DEBUG

int main(int argc, char ** argv) {
  get_args(argc, argv);

  do_work();
  return 0;
}

void get_args(int argc, char ** argv) {
  if (argc < 4) {
    printf("Usage: <max work> <consumer threads> <input file> ...");
    exit(1);
  }
  maxwork = strtol(argv[1], NULL, 10);
  consumers = strtol(argv[2], NULL, 10);
  producers = argc - 3; // One producer thread per input file
  input_files = &argv[3];
  #ifdef DEBUG
  printf("max work: %d consumers: %d producers: %d\nfiles:\n", maxwork, consumers, producers);
  int i;
  for (i = 0; i < producers; i++) {
    printf("[%d] %s\n", i, input_files[i]);
  }
  #endif
}

void do_work() {
  int threads = consumers + producers;
  omp_set_num_threads(threads);

  #pragma omp parallel
  {
    #ifdef DEBUG
      #pragma omp single
      {
        int tc = omp_get_num_threads();
        printf("%d threads created\n", tc);
      }
      #pragma omp barrier
    #endif
    int rank = omp_get_thread_num();
  }
}
