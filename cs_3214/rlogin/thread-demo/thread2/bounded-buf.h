#include <stdbool.h>

#define BOUNDED_BUFFER_SIZE 150

struct bounded_buffer {
  int size;
  int head;
  int tail;
  int num[BOUNDED_BUFFER_SIZE];
};

typedef struct bounded_buffer bounded_buffer_t;

void bounded_buffer_init(bounded_buffer_t *bufp);

bool bounded_buffer_emtpy(bounded_buffer_t *bufp);

bool bounded_buffer_full(bounded_buffer_t *bufp);

int  bounded_buffer_size(bounded_buffer_t *bufp);

void bounded_buffer_add(bounded_buffer_t *bufp, int n);

int  bounded_buffer_remove(bounded_buffer_t *bufp);

