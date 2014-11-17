#include <stdio.h>
#include <ctype.h>
#include "Q3.h"

int main(int argc, char** argv) {

   char mc = *argv[1];
   if ( Q3(mc) ) {
      printf("%c worked\n", mc);
   }
   else printf("It didn't work!!!\n");

   return 0;
}

