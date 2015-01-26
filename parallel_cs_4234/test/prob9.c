#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include "omp.h"
#include "list.h"
#define LINE_SZ 64

void start_consumer(struct list * queue, omp_lock_t * lock);
void start_producer(struct list * queue, int thread_count, omp_lock_t * lock);

int myid, numprocs, task_num, task_count = 0, done = 0;
omp_lock_t queue_lock;
struct list * msg_queue;
char * process_msg;
struct msg { char * msg; struct list_elem elem; };

int main(int argc, char ** argv) {
  task_num = 2;
  omp_init_lock(&queue_lock);
  msg_queue = (struct list *) malloc(sizeof(struct list));
  process_msg = (char *) calloc(LINE_SZ * 2, sizeof(char));
  list_init(msg_queue);

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  omp_set_nested(1);
  #pragma omp parallel sections shared(msg_queue, task_num, queue_lock)
  {
    #pragma omp section
    start_consumer(msg_queue, &queue_lock);
    #pragma omp section
    start_producer(msg_queue, task_num, &queue_lock);
  }
  // printf("Send: %s\n", process_msg);
  MPI_Send(process_msg, LINE_SZ * 2, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  if (myid == 0) {
    int i; char * temp = calloc(LINE_SZ * 2, sizeof(char));
    for (i =0; i < numprocs; i++) {
      MPI_Recv(temp, LINE_SZ * 2, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n", temp);
    }
  }
  MPI_Finalize();
  return 0;
}


void start_consumer(struct list * queue, omp_lock_t * lock) {
  int counter = 0;
  while (true) {
    omp_set_lock(lock);
    if (list_size(queue) > 0) {
      struct list_elem * curr_elem = list_pop_front(queue);
      struct msg * curr_msg = list_entry(curr_elem, struct msg, elem);
      if (counter == 0) { sprintf(process_msg, "%s\n", curr_msg->msg); }
      else { sprintf(process_msg + strlen(process_msg), "%s", curr_msg->msg); }
      free(curr_msg->msg); free(curr_msg); counter++;
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
    omp_set_lock(lock);
    if (task_count == 0) { sprintf(my_msg->msg, "Hello from %d", myid); }
    else { sprintf(my_msg->msg, "Numprocs is %d", numprocs); }
    list_push_back(queue, &my_msg->elem);
    task_count++; if (task_count == thread_count) { done = 1; }
    omp_unset_lock(lock);
  }
}
