#ifndef QUEUE_H
#define QUEUE_H	
#include <stddef.h>
#include <stdbool.h>

/* Our queues have two Guard elements: fGuard just before the first
   interior element (if any) and rGuard just after the last interior
   element (if any).  

   The 'prev' link of the front guard is NULL, as is the 'next' link of the 
   rear guard.  Their other two links point toward each other (directly, or
   via the interior elements of the queue).

   An empty list looks like this:

                      +------------+     +-----------+
                 NULL-| fGuard     |<--->| rGuard    |-NULL
                      +------------+     +-----------+

   A queue with two elements in it looks like this:

           +------------+     +-------+     +-------+     +-----------+
      NULL-| fGuard     |<--->|   1   |<--->|   2   |<--->| rGuard    |-NULL
           +------------+     +-------+     +-------+     +-----------+

   The symmetry of this arrangement eliminates lots of special cases in queue
   processing.  

   (Because only one of the pointers in each guard element is used, we could 
   in fact combine them into a single header element without sacrificing this 
   simplicity.  But using two separate elements allows us to do a little bit 
   of checking on some operations, which can be valuable.) 

   This implementation of a queue does not require use of dynamically allocated
   memory.  Instead, each structure that is a potential list element must embed
   a QNode member.  All of the list functions operate on these QNodes.  

   The QList_Entry macro allows conversion from a QNode back to a structure object 
   that contains it.

   For example, suppose there is a need for a queue of 'struct Widget'.
   'struct Widget' should contain a 'QNode' member, like so:

      struct Widget {
         QNode node;
         int bar;
         ...other members...
      };

   Then a list of 'struct Widget' can be be declared and initialized like so:

      Queue Widget_Q;

      Queue_Init(&Widget_Q);

   Retrieval is a typical situation where it is necessary to convert from a 
   QNode back to its enclosing structure.  Here's an example using Widget_Q:

      QNode *e = Queue_Pop(Widget_Q);

      struct Widget *f = Queue_Entry(e, struct Widget, node);
      // now, do something with f...

   The interface for this queue is inspired by the queue<> template in the C++ 
   STL.  If you're familiar with queue<>, you should find this easy to use.  
   However, it should be emphasized that these queues do *no* type checking and 
   can't do much other correctness checking.  If you screw up, it will bite you.

   Glossary of Queue terms:

     - "interior element": An element that is not the head or tail, that is, a 
               real queue element.  An empty queue does not have any interior elements.

     - "front": The first interior element in a queue.  Undefined in an empty queue.
                Returned by Queue_Front().

     - "back": The last interior element in a queue.  Undefined in an empty list.
               Returned by Queue_Back().

     - "end":  The element figuratively just after the last interior element of a queue;
               i.e., the rear guard.  
               Well-defined even in an empty queue.
               Returned by Queue_End().
*/

// Queue node:
struct _QNode {

   struct _QNode *prev;     /* Previous list element. */
   struct _QNode *next;     /* Next list element.    */
};

typedef struct _QNode QNode;

// Queue object:
struct _Queue {
   QNode fGuard;    // sentinel node at the front of the queue
   QNode rGuard;     // sentinel node at the tail of the queue
};

typedef struct _Queue Queue;

// Queue_Entry() is a useful macro; there is a full discussion of a similar
// macro for a generic doubly-linked list implementation in the CS 2505 notes.
// Converts pointer to queue element NODE into a pointer to the structure that
// NODE is embedded inside.  Supply the name of the outer structure STRUCT and 
// the member name MEMBER of the NODE.  See the big comment at the top of the
// file for an example. 

#define Queue_Entry(NODE, STRUCT, MEMBER)                              \
        ((STRUCT *) ((uint8_t *) (NODE) - offsetof (STRUCT, MEMBER)))

// Initialize QNode pointers to NULL.
//
// Pre:  pN points to a QNode object
// Post: pN->prev and pN->next are NULL
//
void QNode_Init(QNode* const pN);

// Initialize Queue to empty state.
//
// Pre:  pQ points to a Queue object
// Post: *pQ has been set to an empty state (see preceding comment
//
void Queue_Init(Queue* const pQ);

// Return whether Queue is empty.
//
// Pre:  pQ points to a Queue object
// Returns true if *pQ is empty, false otherwise
//
bool Queue_Empty(const Queue* const pQ);

// Insert *pNode as last interior element of Queue.
//
// Pre:  pQ points to a Queue object
//       pNode points to a QNode object
// Post: *pNode has been inserted at the rear of *pQ
//
void Queue_Push(Queue* const pQ, QNode* const pNode);

// Remove first interior element of Queue and return it.
//
// Pre:  pQ points to a Queue object
// Post: the interior QNode that was at the front of *pQ has been removed
// Returns pointer to the QNode that was removed, NULL if *pQ was empty
//
QNode* const Queue_Pop(Queue* const pQ);

// Return pointer to the first interior element, if any.
//
// Pre:  pQ points to a Queue object
// Returns pointer first interior QNode in *pQ, NULL if *pQ is empty
//
QNode* const Queue_Front(const Queue* const pQ);

// Return pointer to the last interior element, if any.
//
// Pre:  pQ points to a Queue object
// Returns pointer last interior QNode in *pQ, NULL if *pQ is empty
//
QNode* const Queue_Back(const Queue* const pQ);

// Return pointer to the rear guard; useful for traversal logic.
//
// Pre:  pQ points to a Queue object
// Returns pointer rear guard element.
//
const QNode* const Queue_End(const Queue* const pQ);

#endif

