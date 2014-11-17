#include <stdlib.h>
#include "IntegerDT.h"

	// Insert K into payload field and set node pointers to NULL.
	//
	// Pre:  pDT points to an IntegerDT object
	// Post: pDT->payload == K
	//       pDT->node has its pointer fields set to NULL
	//
void IntegerDT_Init(IntegerDT* const pDT, int32_t K) {
	QNode *pNode = malloc(sizeof(QNode));
	QNode_Init(pNode);
	pDT->payload = K;
	pDT->node = *pNode;
}