#include <stdlib.h>
#define CANARY	0XDEADBEEF

#include "WriteSquares.h"

int main() {

   int  Sz = 100;
   int* MemoryBlock = malloc(Sz * sizeof(int) + 8);
   if ( MemoryBlock == NULL ) return 1;

   *MemoryBlock = CANARY;
   *(MemoryBlock + 101) = CANARY;
   int* A = MemoryBlock + 1;

   // We suspect this function may contain a bug (or two),
   // and it may write outside the proper boundaries of the
   // array A of dimension Sz:
   WriteSquares(A, Sz);

   free(MemoryBlock);   
   return 0;
}

