/*

 Trivial MPI example.

 Adds up 1000 numbers in an array, where the values are first
 read from a file.

*/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXSIZE 1000

int main(int argc, char *argv[])
{
   int myid, numprocs;
   int data[MAXSIZE], i, x, low, high, myresult=0, result=0;
   char fn[255] = "rand_data.txt";
   FILE *fp;

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);

   if (myid == 0)  {      /* Open input file and initialize data */
      if ((fp = fopen(fn,"r")) == NULL) {
         printf("Can't open the input file: %s\n\n", fn);
         exit(1);
      }
      for(i=0; i<MAXSIZE; i++) fscanf(fp,"%d", &data[i]);
   }

   /* broadcast data */
   MPI_Bcast(data, MAXSIZE, MPI_INT, 0, MPI_COMM_WORLD);

   /* Add my portion of data */
   x = MAXSIZE/numprocs;            /* assumes this divides evenly */
   low = myid * x;
   high = low + x;
   for(i=low; i<high; i++)
     myresult += data[i];
   printf("I got %d from %d\n", myresult, myid);

   /* Compute global sum */
   MPI_Reduce(&myresult, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   if (myid == 0) printf("The sum is %d.\n", result);

   MPI_Finalize();
}
