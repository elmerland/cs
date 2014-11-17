#include <stdbool.h>
#include "bounded-buf.h"
#include <assert.h>


bool bounded_buffer_empty(bounded_buffer_t *bufp) {
  if(bufp->size==0) return true;
  else              return false;
}

bool bounded_buffer_full(bounded_buffer_t *bufp) {
  if(bufp->size == BOUNDED_BUFFER_SIZE) return true;
  else                                  return false;
}

void bounded_buffer_init(bounded_buffer_t *bufp) {
  bufp->size = 0;
  bufp->head = 0;
  bufp->tail = 0;
}

int bounded_buffer_size(bounded_buffer_t *bufp) {
  return bufp->size;
}

void bounded_buffer_add(bounded_buffer_t *bufp, int n) {
  int sz =bufp->size;
  bufp->num[bufp->tail]=n;
  bufp->tail = (bufp->tail+1)%BOUNDED_BUFFER_SIZE;
  bufp->size = sz+1;
}

int bounded_buffer_remove(bounded_buffer_t *bufp) {
  int sz = bufp->size;
  int n = bufp->num[bufp->head];
  bufp->head = (bufp->head+1)%BOUNDED_BUFFER_SIZE;
  bufp->size = sz-1;
  return n;
}

