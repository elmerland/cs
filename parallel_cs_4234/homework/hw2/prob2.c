#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VEC_SIZE 51700
#define ROOT 0

int myid, numprocs;

/**
 * Add vector x and y into z
 * @param x Vector x
 * @param y Vector y
 * @param z Vector z
 * @param n Number of elements in each vector
 */
void vector(double x[], double y[], double z[], int n);

/**
 * Get displacement (offsets) and count information for all processes
 * @param processes The number of processes
 * @param values    The number of total values
 * @param offsets   Array where the offsets will be stored
 * @param counts    Array where the counts with be stored
 */
void get_scatter_info(int processes, int values, int displs[], int counts[]);

/**
 * Get the start and end index for process 'p_id'
 * @param p_id      The id of the processes
 * @param processes The number of processes
 * @param values    The number of total values
 * @param start     The pointer where the start index will be stored
 * @param end       The pointer where the end index will be stored
 */
void get_offset(int p_id, int p_num, int v_num, int * idxs, int * idxe);

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv); // Initialize communicator
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs); // Get communicator size
  MPI_Comm_rank(MPI_COMM_WORLD, &myid); // Get process rank

  if (myid == ROOT) {
  	// Generate vectors to add
  	double * x = (double*)malloc(VEC_SIZE * sizeof(double));
  	double * y = (double*)malloc(VEC_SIZE * sizeof(double));
  	double * z = (double*)malloc(VEC_SIZE * sizeof(double));
  	// Populate vectors with random data
  	srand(time(0));
  	int i;
  	for (i = 0; i < VEC_SIZE; i++) {
  		x[i] = (double)(rand() % 10);
  		y[i] = (double)(rand() % 10);
  	}
  	// Add vectors
  	vector(x, y, z, VEC_SIZE);
  	// Check sum
  	for (i = 0; i < VEC_SIZE; i++) {
		if (z[i] != (x[i] + y[i])) {
			printf("Error in sum at index %d\n", i);
			break;
		}
	}
	printf("Sum is correct\n");
	// Free vectors
  	free(x);
  	free(y);
  	free(z);
  } else {
  	vector(NULL, NULL, NULL, VEC_SIZE);
  }

  MPI_Finalize();
  return 0;
}

void vector(double x[], double y[], double z[], int n) {
	// Get the displacements and send counts for all processes.
	int offsets[numprocs], counts[numprocs];
	get_scatter_info(numprocs, n, offsets, counts);
	// Generate the receive buffers
	int partial_sz = counts[myid];
	double partial_x[partial_sz];
	double partial_y[partial_sz];
	// Scatter vectors
	MPI_Scatterv(x, counts, offsets, MPI_DOUBLE,
		partial_x, partial_sz, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
	MPI_Scatterv(y, counts, offsets, MPI_DOUBLE,
		partial_y, partial_sz, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
	// Sum partial vectors
	double result[partial_sz];
	int i;
	for (i = 0; i < partial_sz; i++) {
		result[i] = partial_x[i] + partial_y[i];
	}
	// Gather result
	MPI_Gatherv(result, counts[myid], MPI_DOUBLE,
		z, counts, offsets, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
}

void get_scatter_info(int processes, int values, int offsets[], int counts[]) {
   int p_id;
   for (p_id = 0; p_id < processes; p_id++) {
      int start, end;
      get_offset(p_id, processes, values, &start, &end);
      offsets[p_id] = start;
      counts[p_id] = end - start;
   }
}

void get_offset(int p_id, int processes, int values, int * start, int * end) {
	int r = values % processes;
	int offset, step, idx;
	if (p_id < r) {
		offset = 0;
		step = (values / processes) + 1;
		idx = p_id;
	} else {
		offset = ((values / processes) + 1) * r;
		step = values / processes;
		idx = p_id - r;
	}

	*start = offset + (step * idx);
	*end = (*start) + step;
}
