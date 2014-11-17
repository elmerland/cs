#include "Queue.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

int main() {
	printf("Hellow World!\n");
	
	Queue pQ;
	Queue_Init(&pQ);
	
	QNode node;
	Queue_Push(&pQ, &node);
	
	printf("fGurad:\t%p\n", &pQ.fGuard);
	printf("rGuard:\t%p\n\n", &pQ.rGuard);
	
	printf("Push");
	printf("node:\t%p\n", &node);
	printf("node.next:\t%p\n", node.next);
	printf("node.prev:\t%p\n\n", node.prev);
	
	QNode *frontNode = Queue_Front(&pQ);
	printf("Front");
	printf("frontNode:\t%p\n\n", frontNode);
	
	QNode *backNode = Queue_Back(&pQ);
	printf("Back");
	printf("backNode:\t%p\n\n", backNode);
	
	Queue_Pop(&pQ);
	printf("Pop");
	printf("fGuard.next:\t%p\n", pQ.fGuard.next);
	printf("rGuard.prev:\t%p\n", pQ.rGuard.prev);
	
	
	return 0;
}