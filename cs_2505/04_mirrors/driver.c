// Test driver for HW 4, CS 2505 Summer I 2013
//
// Implement your solution in findMirrors.c and compile with the command:
//
//    gcc -o test4 -std=c99 -Wall driver.c findMirrors.c
//
// If no errors occur, this will produce an executable named test4, which
// you can execute from the command line:
//
//    linux> ./test4
//
// You can, and should, test with different arrays by altering the declarations
// of sz and a[] in the code below, and then recompiling.
//
#include <stdio.h>          // for standard I/O library functions
#include <stdint.h>         // for C99 standard integer types
#include <inttypes.h>       // for I/O features for stdint.h types
#include "findMirrors.h"    // for declaration of the function you're doing

// declaration of utility function used by main():
void printList(const int32_t a[], uint32_t sz);

int main() {
	
	uint32_t sz = 6;                           // dimension of array to be used
	//          0  1  2  3  4  5  6  7  8
	int a[6] = {3, 2, 1, 1, 2, 3};    // array to be tested

	uint32_t report = findMirrors(a, sz);      // call your implementation

   // reconstruct the mirror bounds and length:
   uint32_t rightStart = report % 1000;
   uint32_t leftStart = (report / 1000 ) % 1000;
   uint32_t matchLength = (report / 1000000) % 1000;

   // print out the results
	printf("report:      %"PRIu32"\n", report);
	printf("leftStart:   %"PRIu32"\n", leftStart);
	printf("rightStart:  %"PRIu32"\n", rightStart);
	printf("length:      %"PRIu32"\n", matchLength);
	
	return 0;
}

/**
 *  Prints the contents of an array, with index numbers.
 *  Pre:
 *        sz == 0 or a[0:sz-1] contain data
 *  Post:
 *        If sz > 0, a[0:sz-1] have been printed, formatted as:
 *
 *     0     1     2     3   ...   sz-1
 *  a[0]  a[1]  a[2]  a[3]   ...   a[0]
 */
void printList(const int32_t a[], uint32_t sz) {

   for (int idx = 0; idx < sz; idx++) {
      printf("%5d", idx);
   }
   printf("\n");

   for (int idx = 0; idx < sz; idx++) {
      printf("%5"PRId32, a[idx]);
   }
   printf("\n");
}
