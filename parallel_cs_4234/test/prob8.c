#include <stdlib.h>
#include <stdio.h>
#include "omp.h"
#include "list.h"
#define LINE_SZ 64

void start_consumer(struct list * queue, omp_lock_t * lock);
void start_producer(struct list * queue, int thread_count, omp_lock_t * lock);

int task_num, task_count = 0, done = 0;
omp_lock_t queue_lock;
struct list * msg_queue;
struct msg { char * msg; struct list_elem elem; };

int main(int argc, char ** argv) {
  task_num = strtol(argv[1], NULL, 10);
  omp_init_lock(&queue_lock);
  msg_queue = (struct list *) malloc(sizeof(struct list));
  list_init(msg_queue);

  omp_set_nested(1);
  #pragma omp parallel sections shared(msg_queue, task_num, queue_lock)
  {
    #pragma omp section
    start_consumer(msg_queue, &queue_lock);
    #pragma omp section
    start_producer(msg_queue, task_num, &queue_lock);
  }
  return 1;
}


void start_consumer(struct list * queue, omp_lock_t * lock) {
  while (true) {
    omp_set_lock(lock);
    if (list_size(queue) > 0) {
      struct list_elem * curr_elem = list_pop_front(queue);
      struct msg * curr_msg = list_entry(curr_elem, struct msg, elem);
      printf("%s\n", curr_msg->msg);
      free(curr_msg->msg); free(curr_msg);
    } else if (done) {omp_unset_lock(lock); return;}
    omp_unset_lock(lock);
  }
}

void start_producer(struct list * queue, int thread_count, omp_lock_t * lock) {
  omp_set_dynamic(0);
  #pragma omp parallel num_threads(thread_count) shared(task_count)
  {
    struct msg * my_msg = (struct msg *) malloc(sizeof(struct msg));
    my_msg->msg = (char *) calloc(LINE_SZ, sizeof(char));
    sprintf(my_msg->msg, "Hello from Task %d", omp_get_thread_num());
    omp_set_lock(lock);
    list_push_back(queue, &my_msg->elem);
    task_count++; if (task_count == thread_count) { done = 1; }
    omp_unset_lock(lock);
  }
}
