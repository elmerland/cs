#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>
#include <unistd.h>
#include <sched.h>
#include "list.h"

#define RANK omp_get_thread_num() // Thread rank in team
#define READ_ONLY "r" // Read only permsion for input files
#define LINE_SZ 256 // Maximum size for a line
#define MAX_LIST_SZ 200 // Maximum size for the list queue
#define DELIM " \t\n" // Delimeter characters for words

struct line {
  char * content;
  struct list_elem elem;
};

void get_args(int argc, char ** argv);
void do_work(void);
void launch_producers(void);
void launch_consumers(void);
int produce(FILE * file, omp_lock_t * list_lock, int line_num);
int consume(omp_lock_t * list_lock);

int _consumers = 0, _producers = 0, _producers_done = 0, _list_size = 0, _is_done = false;
char ** _input_files;
struct list _queue;
omp_lock_t _list_lock;

int main(int argc, char ** argv) {
  get_args(argc, argv);
  omp_init_lock(&_list_lock); // Lock for list access and list size counter
  list_init(&_queue);

  omp_set_dynamic(0); // Only allow set number of threads
  omp_set_nested(1); // Allow nested parallel blocks
  #pragma omp parallel num_threads(2) default(none) shared(_is_done)
    do_work();
  return 0;
}

void do_work(void) {
  #pragma omp sections
  {
    #pragma omp section
      launch_producers();
    #pragma omp section
      launch_consumers();
  }
}

void launch_producers(void) {
  #pragma omp parallel num_threads(_producers) default(none) \
    shared(_input_files, _producers, _producers_done, _list_lock, _list_size, _queue)
  {
    // Get and open input file
    FILE * file = fopen(_input_files[RANK], READ_ONLY);
    if (file == NULL) {
      perror("Could not open file");
      exit(1);
    }
    // Produce
    long curr_work = 0;
    while (1) {
      if (produce(file, &_list_lock, curr_work)) break; // Reached end of file
    }

    // Close input file
    if (fclose(file)) {
      perror("Could not close file");
      exit(1);
    }
  }
  _is_done = true;
}

void launch_consumers(void) {
  #pragma omp parallel num_threads(_consumers) default(none) \
    shared(_queue, _producers, _producers_done, _list_lock, \
    _list_size, _is_done)
  {
    // Consume lines from queue until done
    while (1) {
      if (consume(&_list_lock)) break;
    }
  }
}

int produce(FILE * file, omp_lock_t * list_lock, int line_num) {
  // Read line
  char * my_line = (char* ) malloc(sizeof(char) * LINE_SZ); // Malloc line
  if (fgets(my_line, LINE_SZ, file) == NULL) { // Test if EOF
    free(my_line);
    return -1;
  }
  // Create line struct to add to queue
  struct line * new_line = (struct line *) malloc(sizeof(struct line));
  new_line->content = my_line;

  // Add line to queue. Loop until successful.
  while (1) {
    omp_set_lock(list_lock);
    if (_list_size < MAX_LIST_SZ) { // Test if list is full
      list_push_back(&_queue, &new_line->elem); // Push to end of list
      _list_size++;
      omp_unset_lock(list_lock);
      break;
    }
    omp_unset_lock(list_lock);
    sched_yield(); // Yield for other threads while list is full
  }
  return 0;
}

int consume(omp_lock_t * list_lock) {
  struct list_elem * elem;
  // Read line from queue. Loop until successful.
  while (1) {
    omp_set_lock(list_lock);
    if (_list_size > 0) { // Test if list is empty
      elem = list_pop_front(&_queue); // Pop front element
      _list_size--;
      omp_unset_lock(list_lock);
      break;
    } else if (_is_done) {
      omp_unset_lock(list_lock);
      return -1;
    }
    omp_unset_lock(list_lock);
    sched_yield(); // Yield for other threads while list is full
  }
  // Count words
  struct line * curr_line = list_entry(elem, struct line, elem); // Get line
  char * savptr, * token;
  token = strtok_r(curr_line->content, DELIM, &savptr);
  while (token != NULL) { // Iterate through all tokens
    printf("%s\n", token); // Print out word
    token = strtok_r(NULL, DELIM, &savptr);
  }
  free(curr_line->content);
  free(curr_line);
  return 0;
}

void get_args(int argc, char ** argv) {
  if (argc < 3) {
    printf("Usage: <consumer_threads> <input file> ...");
    exit(1);
  }
  _consumers = strtol(argv[1], NULL, 10);
  _producers = argc - 2; // One producer thread per input file
  _input_files = &argv[2]; // Copy pointer to input file names
}
