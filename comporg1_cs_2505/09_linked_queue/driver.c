#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include "IntegerQueue.h"
#include "IntegerDT.h"
#include "CmdFileParser.h"

void printQueue(FILE* fp, const Queue* const pQ);

int main(int argc, char** argv) {

   IntegerQueue myQueue;
   IntegerQueue_Init(&myQueue);

   if ( !setCmdFile("CmdFile01.txt") ) {
      printf("Error opening commands file.\n");
      return 1;
   }
   
   int32_t value;
   Command nextCmd = getNextCommand();
   while ( strcmp(nextCmd.word, "quit") != 0 ) {
      if ( strcmp(nextCmd.word, "push") == 0 ) {
         fprintf(stdout, "Pushing value: %"PRId32"\n", nextCmd.parameter);
         IntegerQueue_Push(&myQueue, nextCmd.parameter);
      }
      else if ( strcmp(nextCmd.word, "pop") == 0 ) {
         value = IntegerQueue_Pop(&myQueue);
         fprintf(stdout, "Popped value: %"PRId32"\n", value);
      }
      else if ( strcmp(nextCmd.word, "show") == 0 ) {
         fprintf(stdout, "Current queue contents:\n");
         printQueue(stdout, &myQueue.Q);
      }
      else {
         fprintf(stdout, "Unrecognized command: %s\n", nextCmd.word);
      }
      nextCmd = getNextCommand();
   }
   
//   printf("Reported list size after setup: %d\n", DList_Size(&myList));
   printQueue(stdout, &myQueue.Q);

   return 0;
}

void printQueue(FILE* fp, const Queue* const pQ) {

   int i = 0;
   QNode* pCurrNode = pQ->front.next;
   while ( pCurrNode != &pQ->rear ) {
      fprintf(fp, "%5d:  ", i);
      IntegerDT* pCurrWrapper = Queue_Entry(pCurrNode, IntegerDT, node);
      fprintf(fp, "%"PRId32, pCurrWrapper->payload);
      fprintf(fp, "\n");

      i++;
      pCurrNode = pCurrNode->next;
   }
}

/*
void printReversedList(FILE* fp, const DList* const pList) {

   int i = 0;
   DNode* pCurrNode = pList->tail.prev;
   while ( pCurrNode != &pList->head ) {
      fprintf(fp, "%5d:  ", i);
      IntegerDT* pCurrWrapper = DList_Entry(pCurrNode, IntegerDT, node);
      fprintf(fp, "%"PRId32, pCurrWrapper->payload);
      fprintf(fp, "\n");

      i++;
      pCurrNode = pCurrNode->prev;
   }
}
*/
