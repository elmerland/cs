
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXSIZE 1000
#define ROOT 0

/**
 * Get displacement (offsets) and count information for all processes
 * @param processes The number of processes
 * @param values    The number of total values
 * @param offsets   Array where the offsets will be stored
 * @param counts    Array where the counts with be stored
 */
void get_scatter_info(int processes, int values, int offsets[], int counts[]);

/**
 * Get the start and end index for process 'p_id'
 * @param p_id      The id of the processes
 * @param processes The number of processes
 * @param values    The number of total values
 * @param start     The pointer where the start index will be stored
 * @param end       The pointer where the end index will be stored
 */
void get_offset(int p_id, int processes, int values, int * start, int * end);

int main(int argc, char *argv[])
{
   int myid, numprocs;
   int data[MAXSIZE], i, myresult = 0, result = 0;
   char fn[255] = "rand_data.txt";
   FILE *fp;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);

   if (myid == 0)  {      /* Open input file and initialize data */
      if ((fp = fopen(fn, "r")) == NULL) {
         printf("Can't open the input file: %s\n\n", fn);
         exit(1);
      }
      for(i = 0; i < MAXSIZE; i++) {
         fscanf(fp, "%d", &data[i]);
      }
   }
   
   // Get displacements and send counts for all processes
   int offsets[numprocs], counts[numprocs];
   get_scatter_info(numprocs, MAXSIZE, offsets, counts);
   // Generate receive buffers
   int partial_sz = counts[myid];
   int partial[partial_sz];
   MPI_Scatterv(data, counts, offsets, MPI_INT,
      partial, partial_sz, MPI_INT, ROOT, MPI_COMM_WORLD);

   for(i = 0; i < partial_sz; i++) {
      myresult += partial[i];
   }
   printf("I got %d from %d\n", myresult, myid);

   // Compute global sum
   MPI_Reduce(&myresult, &result, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);

   if (myid == 0) {
      printf("The sum is %d.\n", result);
   }

   MPI_Finalize();
   return 0;
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
