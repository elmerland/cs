#include "uthreads.h"
#include "list.h"

void uthreads_mutex_init(uthreads_mutex_t m)
{
    m->holder = NULL;
    list_init(&m->waiters);
}

void uthreads_mutex_lock(uthreads_mutex_t m)
{
	// Check if some other thread has lock
	if (m->holder == NULL) {
		// No thread has lock.
		m->holder = uthreads_current();
	} else {
		// Other thread has lock, add self to waiting list.
		uthreads_t this_thread = uthreads_current();
		list_push_back(&m->waiters, &(this_thread->elem));
		uthreads_block();
	}
}

void uthreads_mutex_unlock(uthreads_mutex_t m)
{
	// Check if the waiter list is empty
	if (!list_empty(&m->waiters)) {
		// Set the front element as the new lock holder.
		// Pop the front waiter
		struct list_elem * front_elem = list_pop_front(&m->waiters);
		uthreads_t front = list_entry(front_elem, struct thread_control_block, elem);
		// Set front element to lock holder
		m->holder = front;
		uthreads_unblock(front);
	} else {
		// Set the lock holder to null
		m->holder = NULL;
	}
}
