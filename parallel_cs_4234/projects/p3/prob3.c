#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>

// #define MEM_FP 10000 // 10 KiloByte
// #define MEM_FP 100000 // 100 KiloByte
// #define MEM_FP 1000000 // 1 MegaByte
#define MEM_FP 10000000 // 10 MegaByte
#define MAX_WORD_SZ 128
#define IS_DELIM(c) (c == '\n' || c == '\t' || c == '\r' || c == ' ')
// #define TIMING
#define OUTPUT

void do_work(void);
void consume_blks(char * file_name, int blk_num);
void get_blocksz(void);
void get_file_offsets(int blk_num, int * start, int * end);
void get_args(int argc, char ** argv);

int _min_blksz = MAX_WORD_SZ + 1,
    _max_blksz,     // Maximum size of a block
    _num_blks,      // The number of blocks to process
    _curr_blk = 0,  // The current block to process
    _thread_count;  // The thread count

long _file_sz;       // The size of the file
int * _incomplete;
int * _freq_table;
char * _file_name;


int main(int argc, char ** argv) {
  get_args(argc, argv);
  #ifdef TIMING
  double start_time = omp_get_wtime();
  #endif
  // Open file and get size
  FILE * file = fopen(_file_name, "r");
  if (file == NULL) {
    perror("Could not open file");
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  _file_sz = ftell(file) / sizeof(char);
  fclose(file);
  get_blocksz();
  _incomplete = (int *) calloc(_num_blks + 1, sizeof(int));
  _freq_table = (int *) calloc(MAX_WORD_SZ + 1, sizeof(int));
  do_work();

  // Add incomplete words
  int i;
  for (i = 0; i < _num_blks + 1; i++) {
    if (_incomplete[i] != 0) {
      _freq_table[_incomplete[i]]++;
    }
  }

  #ifdef TIMING
  double end_time = omp_get_wtime();
  printf("Parallel %d & %f sec &\n", MEM_FP, end_time - start_time);
  #endif

  // Print frequency table
  #ifdef OUTPUT
  printf("Alphabets    Frequency\n");
  printf("----------------------\n");
  for (i = 0; i < MAX_WORD_SZ + 1; i++) {
    if (_freq_table[i] != 0) {
      printf("%3d          %d\n", i, _freq_table[i]);
    }
  }
  #endif
  return 0;
}

void do_work(void) {
  omp_set_dynamic(0);
  #pragma omp parallel num_threads(_thread_count) default(none) \
    shared(_file_name, _num_blks, _curr_blk, _incomplete, _freq_table)
  {
    int done = false;
    while (true) {
      int blk_num;
      #pragma omp critical
      {
        if (_curr_blk == _num_blks) {
          done = true;
        } else {
          blk_num = _curr_blk;
          _curr_blk++;
        }
      }
      if (done) { break; }
      else { consume_blks(_file_name, blk_num); }
    }
  }
}

void consume_blks(char * file_name, int blk_num) {
  int start, end, blksz;

  FILE * file = fopen(file_name, "r");
  if (file == NULL) { perror("Could not open file"); exit(1); }

  get_file_offsets(blk_num, &start, &end);
  blksz = end - start;

  fseek(file, start, SEEK_SET); // Move pointer to block start
  char * block = (char *) calloc(blksz + 1, sizeof(char));
  fread(block, sizeof(char), blksz, file); // Read block from file
  int firstword = true, i, count = 0;
  char c;

  for (i = 0; i < blksz; i++) {
    c = block[i];
    if (IS_DELIM(c)) {
      if (firstword) { // First word of block
        if (count > 0) { 
          #pragma omp atomic
            _incomplete[blk_num] += count; // Add to imcomplete list
          count = 0;
        }
        firstword = false;
      } else {
        if (count > 0) { 
          #pragma omp atomic
            _freq_table[count]++;
          count = 0;
        }
      }
    } else { count++; }
  }

  if (count > 0) {
    #pragma omp atomic
      _incomplete[blk_num + 1] += count;
  }

  free(block);
  fclose(file);
}

void get_blocksz(void) {
  // Get block count
  if (MEM_FP/_thread_count <= _min_blksz) {
    _thread_count = MEM_FP / _min_blksz - (MEM_FP % _min_blksz ? 0 : 1);
  }

  _max_blksz = MEM_FP / _thread_count;
  int temp_blksz = _file_sz / _thread_count;

  if (temp_blksz > _max_blksz) {
    _num_blks = _file_sz / _max_blksz + (_file_sz % _max_blksz ? 1 : 0);
  } else if (temp_blksz < _min_blksz) {
    _num_blks = _file_sz / _min_blksz;
    if (_num_blks == 0) _num_blks = 1;
  } else {
    _num_blks = _thread_count;
  }
}

void get_file_offsets(int blk_num, int * start, int * end) {
   int r = _file_sz % _num_blks;
   int offset, step, idx;
   if (blk_num < r) {
      offset = 0;
      step   = (_file_sz / _num_blks) + 1;
      idx    = blk_num;
   } else {
      offset = ((_file_sz / _num_blks) + 1) * r;
      step   = _file_sz / _num_blks;
      idx    = blk_num - r;
   }
   *start = offset + (step * idx);
   *end   = (*start) + step;
}

void get_args(int argc, char ** argv) {
  if (argc < 3) {
    printf("Usage: <thread_count> <input_file>");
    exit(1);
  }
  _thread_count = strtol(argv[1], NULL, 10);
  _file_name = argv[2];
}
