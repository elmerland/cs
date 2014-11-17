#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define NREPS 10
#define NMAX  524288
#define Min(a, b) (a > b ? b : a)

int main(int argc, char *argv[])
{
	int myid, numprocs;
	int i, k, count;
	int *buf;
	double starttime, endtime, delta, mintime;
	MPI_Status status;

	buf = (int *) malloc(NMAX * sizeof(int)); // Allocate buffer
	MPI_Init(&argc, &argv); // Initialize communicator
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs); // Get communicator size
	MPI_Comm_rank(MPI_COMM_WORLD, &myid); // Get process rank

	count = 1;
	for (k = 0; k < 20; k++) {
		mintime = 1000.0;

		for (i = 0; i < NREPS; i++) {
			if (myid == 0) {
				starttime = MPI_Wtime(); // Start clock

				MPI_Send(buf, count, MPI_INT, 1, 0, MPI_COMM_WORLD);
				MPI_Recv(buf, count, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
				// my_bcast(buf, count, MPI_INT, 0, MPI_COMM_WORLD);
				
				endtime = MPI_Wtime(); // Stop clock
				delta = (endtime - starttime) / 2; // Total time per process
				mintime = Min(mintime, delta);
				// printf("%e\n", delta);
			}
			else {
				MPI_Recv(buf, count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
				MPI_Send(buf, count, MPI_INT, 0, 0, MPI_COMM_WORLD);
			}
		}

		if (myid == 0) {
			printf("%8d  %7.3e  %7.3e\n", count, mintime, (4.0 * count) / (mintime * 1024.0 * 1024.0));
		}
		count = count * 2;
	}
	MPI_Finalize();
}