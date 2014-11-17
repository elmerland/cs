#ifndef QUEUE_H
#define QUEUE_H	
#include <stddef.h>
#include <stdbool.h>

/* Our queues have two header elements: the "front" just before the first 
   element and the "rear" just after the last element.  The 'prev' link of 
   the front header is null, as is the 'next' link of the rear header.  
   Their other two links point toward each other via the interior elements 
   of the queue.

   An empty list looks like this:

                      +-------+     +------+
                    [-| front |<--->| rear |-]
                      +-------+     +------+

   A queue with two elements in it looks like this:

        +-------+     +-------+     +-------+     +------+
      [-| front |<--->|   1   |<--->|   2   |<--->| rear |-]
        +-------+     +-------+     +-------+     +------+

   The symmetry of this arrangement eliminates lots of special cases in queue
   processing.  

   (Because only one of the pointers in each header element is used, we could 
   in fact combine them into a single header element without sacrificing this 
   simplicity.  But using two separate elements allows us to do a little bit 
   of checking on some operations, which can be valuable.) 

   This implementation of a queue does not require use of dynamically allocated
   memory.  Instead, each structure that is a potential list element must embed
   a QNode member.  All of the list functions operate on these QNode's.  

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

      QList_Init(&Widget_Q);

   Retrieval is a typical situation where it is necessary to convert from a 
   QNode back to its enclosing structure.  Here's an example using Widget_Q:

      QNode *e = Queue_Pop(Widget_Q);

      struct Widget *f = Queue_Entry (e, struct Widget, node);
      // now, do something with f...

   The interface for this queue is inspired by the queue<> template in the C++ 
   STL.  If you're familiar with queue<>, you should find this easy to use.  
   However, it should be emphasized that these lists do *no* type checking and 
   can't do much other correctness checking.  If you screw up, it will bite you.

   Glossary of Queue terms:

     - "front": The first element in a queue.  Undefined in an empty queue.
                Returned by Queue_Front().

     - "back": The last element in a queue.  Undefined in an empty list.
               Returned by Queue_Back().

     - "tail": The element figuratively just after the lastelement of a queue.  
               Well-defined even in an empty queue.
               Not returned by any Queue function.

     - "head": The element figuratively just before the first element of a queue.
               Well-defined even in an empty list.
               Not returned by any Queue function.

     - "interior element": An element that is not the head or tail, that is, a 
               real queue element.  An empty queue does not have any interior elements.
*/

// Queue node:
struct _QNode {

   struct _QNode *prev;     /* Previous list element. */
   struct _QNode *next;     /* Next list element.    */
};

typedef struct _QNode QNode;

// Queue object:
struct _Queue {
   QNode front;    // sentinel node at the front of the queue
   QNode rear;     // sentinel node at the tail of the queue
};

typedef struct _Queue Queue;

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
void QNode_Init(QNode* pN);

// Initialize Queue to empty state.
//
// Pre:  pQ points to a Queue object
// Post: *pQ has been set to an empty state (see preceding comment
//
void Queue_Init(Queue* pQ);

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

// Return first interior element, if any.
//
// Pre:  pQ points to a Queue object
// Returns pointer first interior QNode in *pQ, NULL if *pQ is empty
//
QNode* const Queue_Front(Queue* const pQ);

// Return last interior element, if any.
//
// Pre:  pQ points to a Queue object
// Returns pointer last interior QNode in *pQ, NULL if *pQ is empty
//
QNode* const Queue_Back(Queue* const pQ);

#endif
