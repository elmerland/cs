#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define READ_ONLY "r" // Read only permsion for input files
#define LINE_SZ 256 // Maximum size for a line
#define DELIM " \t\n"

void get_args(int argc, char ** argv);

int _producers;
char ** _input_files;

int main(int argc, char ** argv) {
  get_args(argc, argv);

  int i;
  for (i = 0; i < _producers; i++) {
    FILE * file = fopen(_input_files[i], READ_ONLY);
    while (1) {
      char line[LINE_SZ];
      if (fgets(line, LINE_SZ, file) == NULL) break;

      char * tok;
      tok = strtok(line, DELIM);
      while (tok != NULL) {
        printf("%s\n", tok);
        tok = strtok(NULL, DELIM);
      }
    }
    
  }

  return 0;
}

void get_args(int argc, char ** argv) {
  if (argc < 2) {
    printf("Usage: <input file> ...");
    exit(1);
  }
  _producers = argc - 1; // One producer thread per input file
  _input_files = &argv[1];

  // int i;
  // for (i = 0; i < _producers; i++) {
  //   printf("[%d] %s\n", i, _input_files[i]);
  // }
}
