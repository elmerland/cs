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

  if (myid == MASTER) printf("Count, time\n", count);
  count = 1;
  for (k = 0; k < 20; k++) {
    mintime = 1000.0;
    for (i = 0; i < NREPS; i++) {
      // Synchronize starting place for all nodes.
      MPI_Barrier(MPI_COMM_WORLD);
      starttime = MPI_Wtime(); // Start clock
      if (myid == MASTER) {
        MPI_Bcast(buf, count, MPI_INT, MASTER, MPI_COMM_WORLD); // Broadcast to all  
      }
      else {
        MPI_Bcast(buf, count, MPI_INT, MASTER, MPI_COMM_WORLD); // Receive broadcast
      }
      MPI_Barrier(MPI_COMM_WORLD);
      // Synchronize stopping place for all nodes.
      endtime = MPI_Wtime(); // Stop clock
      delta = endtime - starttime; // Total broadcast time
      mintime = Min(mintime, delta);
      // printf("%e\n", delta);
    }
    if (myid == MASTER) {
      printf("%8d, %7.3e\n", count, mintime);
    }
    count = count * 2;
  }
  MPI_Finalize();
}