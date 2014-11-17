#ifndef INTEGERQUEUE_H
#define INTEGERQUEUE_H
#include <stdbool.h>
#include <stdint.h>
#include "Queue.h"
#include "IntegerDT.h"

/*  IntegerQueue defines an interface suitable for a client who needs
    to create and manipulate a queue of integer values (int32_t).

    IntegerQueue uses the data wrapper IntegerDT, and the Queue type
    for the underlying physical structure.
*/

struct _IntegerQueue {

   Queue Q;          // generic Queue object
};

typedef struct _IntegerQueue IntegerQueue;

// Initialize empty IntegerQueue.
//
// Pre:  pIQ points to an IntegerQueue object
// Post: pIQ->Q has been initialized to an empty state
//       (see Queue.h for details)
//
void IntegerQueue_Init(IntegerQueue* const pIQ);

// Return whether IntegerQueue is empty.
//
// Pre:  pIQ points to aqn IntegerQueue object
// Returns true if pIQ->Q is empty, false otherwise 
//
bool IntegerQueue_Empty(const IntegerQueue* const pIQ);

// Insert K at rear of IntegerQueue.
//
// Pre:  pIQ points to an IntegerQueue object
// Post: An IntegerDT object containing the value K has been inserted 
//       at the rear of pIQ->Q
//
void IntegerQueue_Push(IntegerQueue* const pIQ, int32_t K);

// Remove and return data value from front of IntegerQueue.
//
// Pre:  pIQ points to a nonempty IntegerQueue object
// Post: The IntegerDT object at the front of pIQ->Q has been removed
// Returns the user data value that was in the IntegerDT object at the 
// front of pIQ->Q
//
int32_t IntegerQueue_Pop(IntegerQueue* const pIQ);

#endif
