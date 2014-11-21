#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char ** argv) {
	int myid, numprocs;
	int i, start, end, delta;
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	/* print out my rank and this run's PE size */
	printf("Hello from %d\n", myid);
	printf("Numprocs is %d\n", numprocs);
	end = MPI_Wtime();
	delta = end - start;
	printf("Time %f\n", delta);
	MPI_Finalize();
}
