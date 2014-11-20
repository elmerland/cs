#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>

#define IS_DELIM(c) (c == '\n' || c == '\t' || c == '\r' || c == ' ')
#define MAX_WORD_SZ 128

// #define TIMING
#define OUTPUT

int main(int argc, char ** argv) {
  #ifdef TIMING
  double start_time = omp_get_wtime();
  #endif

  char * file_name = argv[1];
  FILE * file = fopen(file_name, "r");
  int freq_table[MAX_WORD_SZ + 1] = {0};

  char c;
  int count = 0;
  while (1) {
    c = fgetc(file);
    if (feof(file)) { break; }
    if (IS_DELIM(c)) {
      if (count > 0) { freq_table[count]++; count = 0; }
    } else { count++; }
  }

  if (count > 0) {
    freq_table[count]++;
  }

  #ifdef TIMING
  double end_time = omp_get_wtime();
  printf("Linear   & %f sec & \n", end_time - start_time);
  #endif

  #ifdef OUTPUT
  int i;
  printf("Alphabets    Frequency\n");
    printf("----------------------\n");
  for (i = 0; i < MAX_WORD_SZ + 1; i++) {
    if (freq_table[i] != 0)
      printf("%3d          %d\n", i, freq_table[i]);
  }
  #endif

  return 0;
}
