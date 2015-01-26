#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include "pthread.h"
#include "list.h"
#define LINE_SZ 64

void launch_threads();
void join_threads();
void * start_consumer(void * args);
void * start_producer(void * args);

int myid, numprocs, task_num, task_count = 0, done = 0;
pthread_t * tids;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
struct list * msg_queue;
char * process_msg;
struct msg { char * msg; struct list_elem elem; };

int main(int argc, char ** argv) {
  msg_queue = (struct list *) malloc(sizeof(struct list));
  process_msg = (char *) calloc(LINE_SZ * 2, sizeof(char));
  list_init(msg_queue); task_num = 2;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  launch_threads(); join_threads();
  MPI_Send(process_msg, LINE_SZ * 2, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  if (myid == 0) {
    int i; char * temp = calloc(LINE_SZ * 2, sizeof(char));
    for (i =0; i < numprocs; i++) {
      MPI_Recv(temp, LINE_SZ * 2, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n", temp);
    }
  }
  MPI_Finalize(); return 0;
}

void launch_threads() {
  int thread_num = task_num + 1;
  tids = (pthread_t *) malloc(sizeof(pthread_t) * thread_num); long i;
  for (i = 0; i < thread_num; i++) {
    if (i == 0) { pthread_create(&tids[i], NULL, &start_consumer, NULL); }
    else { pthread_create(&tids[i], NULL, &start_producer, NULL); }
  }
}

void join_threads() {
  int thread_num = task_num + 1; long i;
  for (i = 0; i < thread_num; i++) { pthread_join(tids[i], NULL); }
}

void * start_consumer(void * args) {
  int counter = 0;
  while (true) {
    pthread_mutex_lock(&lock);
    if (list_size(msg_queue) > 0) {
      struct list_elem * curr_elem = list_pop_front(msg_queue);
      struct msg * curr_msg = list_entry(curr_elem, struct msg, elem);
      if (counter == 0) { sprintf(process_msg, "%s\n", curr_msg->msg); }
      else { sprintf(process_msg + strlen(process_msg), "%s", curr_msg->msg); }
      free(curr_msg->msg); free(curr_msg); counter++;
    } else if (done) {pthread_mutex_unlock(&lock); return NULL;}
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

void * start_producer(void * args) {
  struct msg * my_msg = (struct msg *) malloc(sizeof(struct msg));
  my_msg->msg = (char *) calloc(LINE_SZ, sizeof(char));
  pthread_mutex_lock(&lock);
  if (task_count == 0) { sprintf(my_msg->msg, "Hello from %d", myid); }
  else { sprintf(my_msg->msg, "Numprocs is %d", numprocs); }
  list_push_back(msg_queue, &my_msg->elem);
  task_count++; if (task_count == 2) { done = 1; }
  pthread_mutex_unlock(&lock); return NULL;
}
