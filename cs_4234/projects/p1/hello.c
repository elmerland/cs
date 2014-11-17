
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   int myid, numprocs, namelen; 
   char processor_name[MPI_MAX_PROCESSOR_NAME];

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);

   MPI_Get_processor_name(processor_name, &namelen);
   printf("hello from %s: process %d of %d\n", 
           processor_name, myid, numprocs);
 
   MPI_Finalize();
}
