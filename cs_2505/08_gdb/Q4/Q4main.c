#include <stdio.h>
#include <stdlib.h>
#include "Q4.h"

int main(int argc, char** argv) {

   if ( argc != 2 ) {
      printf("Invoke as:  D1 <integer value>\n");
      return 1;
   }

   int mx = atoi(argv[1]);
   int my = Q4(mx);

   printf("my = %d\n", my);

   return 0;
}

