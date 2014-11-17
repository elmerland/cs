#ifndef INTEGERDT_H
#define INTEGERDT_H
#include <stdint.h>
#include "Queue.h"

//  Wrapper type to "duct-tape" an int32_t payload to a QNode.

struct _IntegerDT {
   int32_t payload;    // user data field
   QNode   node;       // generic node
};

typedef struct _IntegerDT IntegerDT;

// Insert K into payload field and set node pointers to NULL.
//
// Pre:  pDT points to an IntegerDT object
// Post: pDT->payload == K
//       pDT->node has its pointer fields set to NULL
//
void IntegerDT_Init(IntegerDT* const pDT, int32_t K);

#endif
