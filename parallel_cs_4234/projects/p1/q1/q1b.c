#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define NREPS 10
#define NMAX  524288
#define Min(a, b) (a > b ? b : a)
#define MASTER 0

int main(int argc, char *argv[])
{
  int myid, numprocs;
  int i, k, count, proc;
  int *buf;
  double starttime, endtime, delta, mintime;
  MPI_Status status;

  buf = (int *) malloc(NMAX * sizeof(int)); // Allocate buffer
  MPI_Init(&argc, &argv); // Initialize communicator
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs); // Get communicator size
  MPI_Comm_rank(MPI_COMM_WORLD, &myid); // Get process rank

  if (myid == MASTER) printf("Count, mintime, bandwidth\n", count);
  count = 1;
  mintime = 1000.0;
  for (k = 0; k < 20; k++) {
    for (i = 0; i < NREPS; i++) {
      MPI_Barrier(MPI_COMM_WORLD);
      starttime = MPI_Wtime();
      if (myid % 2 == 0) {
        MPI_Send(buf, count, MPI_INT, myid + 1, 0, MPI_COMM_WORLD);
      } else {
        MPI_Recv(buf, count, MPI_INT, myid - 1, 0, MPI_COMM_WORLD, &status);
      }
      MPI_Barrier(MPI_COMM_WORLD);
      endtime = MPI_Wtime();
      // Calculations
      delta = (endtime - starttime);
      mintime = Min(mintime, delta);
    }
    if (myid == MASTER) {
      printf("%8d, %7.3e, %7.3e\n", 
      count, mintime, (4.0 * count) / (mintime * 1024.0 * 1024.0));
    }
    count = count * 2;
  }
  MPI_Finalize();
}
