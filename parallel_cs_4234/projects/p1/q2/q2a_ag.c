
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define NREPS 10
#define NMAX  1048576
#define Min(a, b) (a > b ? b : a)
#define MASTER 0

int main(int argc, char *argv[]) {
  int myid, numprocs;
  int i, k, j, seed, count;
  double *data, *data_local, *data_gather;
  struct drand48_data drand_buf;
  double starttime, endtime, mintime;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);

  data       = (double *) malloc(NMAX * sizeof(double));
  data_local = (double *) malloc(NMAX * sizeof(double));
  data_gather = (double *) malloc(NMAX * sizeof(double));

  seed = 1202107158 + MPI_Wtime() * 1999;
  srand48_r (seed, &drand_buf);

  if (myid == 0) {
    for (i = 0; i < NMAX; i++) 
      drand48_r (&drand_buf, &data[i]);
  }

  int chunk_size = NMAX/numprocs;
  MPI_Scatter(data, chunk_size, MPI_DOUBLE,
    data_local, chunk_size, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

  if (myid == MASTER) printf("Count, Time\n");
  count = 1;
  for (k = 0; k < 21; k++) {
    mintime = 1000.;

    for (i = 0; i < NREPS; i++) {
      MPI_Barrier(MPI_COMM_WORLD);
      starttime = MPI_Wtime();

      MPI_Allgather(data_local, count/numprocs, MPI_DOUBLE,
        data_gather, count/numprocs, MPI_DOUBLE, MPI_COMM_WORLD);

      double sum = 0;
      for (j = 0; j < count; j++) {
        sum += data_gather[j];
      }

      MPI_Barrier(MPI_COMM_WORLD);
      endtime = MPI_Wtime();
      mintime = Min(mintime, endtime-starttime);

      double min_sum, max_sum;
      MPI_Reduce(&sum, &min_sum, 1, MPI_DOUBLE, MPI_MIN, MASTER, MPI_COMM_WORLD);
      MPI_Reduce(&sum, &max_sum, 1, MPI_DOUBLE, MPI_MAX, MASTER, MPI_COMM_WORLD);
      if (myid == MASTER && min_sum != max_sum) {
        printf("Sums do not match: %7.3e\t%7.3e\n", min_sum, max_sum);
      }
    }

    if (myid==0) printf("%8d, %7.3e\n", count, mintime);
    count = count*2;
  }

  MPI_Finalize();
}
