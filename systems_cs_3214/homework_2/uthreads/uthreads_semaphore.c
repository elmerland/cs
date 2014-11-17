#include "uthreads.h"

void 
uthreads_sem_init(uthreads_sem_t s, int initial)
{
	s->count = initial;
	list_init(&s->waiters);
}

void 
uthreads_sem_post(uthreads_sem_t s)
{
	s->count++;
	if (!list_empty(&s->waiters)) {
		struct list_elem * front_elem = list_pop_front(&s->waiters);
		uthreads_t front = list_entry(front_elem, struct thread_control_block, elem);
		
		uthreads_unblock(front);
	}
}

void 
uthreads_sem_wait(uthreads_sem_t s)
{
	while(s->count == 0) {
		list_push_back(&s->waiters, &(uthreads_current()->elem));
		uthreads_block();
	}
	s->count--;
}
