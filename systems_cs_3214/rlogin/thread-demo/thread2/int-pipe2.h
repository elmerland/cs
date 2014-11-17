#include <stdbool.h>
#include <pthread.h>
#include "bounded-buf.h"


struct int_pipe {
  bounded_buffer_t buf;
  bool closed;
  pthread_mutex_t mutex;
};

typedef struct int_pipe int_pipe_t;

bool int_pipe_emtpy(int_pipe_t *intp);

bool int_pipe_full(int_pipe_t *intp);

bool int_pipe_closed(int_pipe_t *intp);

void int_pipe_init(int_pipe_t *intp);

int  int_pipe_size(int_pipe_t *intp);

int  int_pipe_write(int_pipe_t *intp, int n);

int  int_pipe_read(int_pipe_t *intp);

void int_pipe_close(int_pipe_t *intp);
