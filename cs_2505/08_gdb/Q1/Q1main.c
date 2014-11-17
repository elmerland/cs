#include <stdio.h>
#include "AddEm.h"

int main() {

   int Sz = 5;
   int A[5] = {32, 16, 64, 4, 8};

   int Sum = AddEm(A, Sz);

   printf("Sum is %d\n", Sum);

   return 0;
}

