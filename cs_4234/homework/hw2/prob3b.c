
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXSIZE 1000
#define ROOT 0

/**
 * Read a chunk of size 'size' into 'chunk'.
 * @param chunk A block of memory of at least 'size' integers.
 * @param file  File pointer from which intergers are going to be read from.
 * @param size  The number of integers to read.
 */
void read_chunk(int * chunk, FILE * file, int size);

/**
 * Get the count info of the chunk size for all processes
 * @param processes The number of processes
 * @param values    The number of total values
 * @param counts    An empty array of size 'processes'
 */
void get_count_info(int processes, int values, int counts[]);

/**
 * Gets the starting and ending index for processes 'p_id'
 * @param p_id      The processes ID
 * @param processes The number of processes
 * @param values    The number of total values
 * @param start     Pointer to where the start index will be stored.
 * @param end       Pointer to where the end index will be stored.
 */
void get_offset(int p_id, int processes, int values, int * start, int * end);

int main(int argc, char *argv[])
{
   int myid, numprocs;
   int * chunk = NULL, i, myresult = 0, result = 0;
   char fn[255] = "rand_data.txt";
   FILE *fp;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);
   
   // Get counts for all processes
   int counts[numprocs];
   get_count_info(numprocs, MAXSIZE, counts);

   // Allocate space for the chunk
   int chunk_sz = counts[myid];
   chunk = (int *)malloc(chunk_sz * sizeof(int));

   if (myid == ROOT) {
      // Open file
      if ((fp = fopen(fn, "r")) == NULL) {
         printf("Can't open the input file: %s\n\n", fn);
         exit(1);
      }
      // Read and send all chunks to all processes
      int i;
      for (i = 0; i < numprocs; i++) {
         // Read the chunk
         read_chunk(chunk, fp, counts[i]);
         // Send a chunk to each thread
         MPI_Send(chunk, counts[i], MPI_INT, i, 0, MPI_COMM_WORLD);
      }
      fclose(fp);
   }

   // Receive chunk
   MPI_Recv(chunk, chunk_sz, MPI_INT, ROOT, 0, MPI_COMM_WORLD, NULL);

   // Sum partial result
   for(i = 0; i < chunk_sz; i++) {
      myresult += chunk[i];
   }
   free(chunk);
   printf("I got %d from %d\n", myresult, myid);

   // Compute global sum
   MPI_Reduce(&myresult, &result, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);

   if (myid == 0) {
      printf("The sum is %d.\n", result);
   }

   MPI_Finalize();
   return 0;
}

void read_chunk(int * chunk, FILE * file, int size) {
   int i;
   for(i = 0; i < size; i++) {
      fscanf(file, "%d", &chunk[i]);
   }
}

void get_count_info(int processes, int values, int counts[]) {
   int p_id;
   for (p_id = 0; p_id < processes; p_id++) {
      int start, end;
      get_offset(p_id, processes, values, &start, &end);
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
