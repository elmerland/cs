#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <omp.h>
#include "mpi.h"

#define IS_ROOT (myid == 0)

struct mult_args {
  double * A;
  double * B;
  double * C;
  int b_row_start;
  int b_col_start;
  int b_col_end;
};

typedef struct mult_args PARAMS;

// Process arguments
void get_args(int argc, char * argv[]);
void print_usage(char * argv[]);

// Read and printing of matrices
void populate_matrices(double * A_blk, double * B_blk);
void read_matrix_file(char * file_name, double * M);
void print_matrix_array(char * m_name, double * m_array, int rows, int columns);

// Ring multiplication for synchronous and asynchronous communication
void async_ring_multiply(int myid, double * A, double * B, double * C);
void sync_ring_multiply(int myid, double * A, double * B, double * C);

// Message passing for synchronous and asynchronous communication
void async_transfer(double * A, int left, int right);
void sync_transfer(double * A, int left, int right, int myid);

// Threaded matrix multiplication
void threaded_multiply(double * A, double * B, double * C, int b_row_start);
void * matrix_multiply(void * args);

// Put and extract information from the arguments to the matrix multiplication
void populate_args(PARAMS * args, double * A, double * B, double * C,
  int b_row_start, int rank);
void unpack_args(void * args, double ** A, double ** B, double ** C,
  int * b_row_start, int * b_col_start, int * b_col_end);


int N;            // Size of matrix
int N_P;          // Size of sub-blocks
int numprocs;     // Number of processes
int myid;         // Process rank
int THREAD_COUNT; // Number of threads
bool ASYNC;       // Asynchronous flag (TRUE: async, FALSE: sync)
bool PTHREAD;     // Pthread flag (TRUE: Pthread, FALSE: OpenMP)
bool READ_FILE;   // Read from file flag (TRUE: read from file, FALSE: generate random numbers)
bool TIME;        // Time flag (TRUE: time multiplication, FALSE: don't time)
double * A_tmp;
double wait_tot = 0, send_tot = 0, recv_tot = 0;
char * matrix_a_file, * matrix_b_file;
PARAMS * param_list;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void get_args(int argc, char * argv[]) {
  ASYNC = false;
  PTHREAD = true;
  THREAD_COUNT = 1;
  READ_FILE = true;
  TIME = false;
  opterr = 0;
  char c;
  while ((c = getopt (argc, argv, "n:am:t:ogeh")) != -1) {
    switch (c) {
      case 'n':
        N = strtol(optarg, NULL, 10);
        break;
      case 'a':
        ASYNC = true;
        break;
      case 't':
        THREAD_COUNT = strtol(optarg, NULL, 10);
        break;
      case 'o':
        PTHREAD = false;
        break;
      case 'g':
        READ_FILE = false;
        break;
      case 'e':
        TIME = true;
        break;
      case 'h':
        print_usage(argv);
        exit(0);
        break;
      case '?':
        if (optopt == 'n' || optopt == 'm' || optopt == 't')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        exit(1);
      default:
        abort ();
    }
  }

  if (READ_FILE) {
    if ((argc- optind) != 2) {
      printf("Invalid number of non-option arguments - optind: %d argc: %d\n", optind, argc);
      int i;
      for (i = optind; i < argc; i++) {
        printf ("%s\n", argv[i]);
      }
      exit(1);
    }
    matrix_a_file = argv[optind];
    matrix_b_file = argv[optind + 1];
  }
}

void print_usage(char * argv[]) {
  printf("Usage: %s [OPTIONS] -- [<input_file_A> <input_file_B>]\n"
      "\t-n <matrix_size>\n\t\tSize of matrices\n"
      "\t-a\n\t\tUse asynchronous message passing\n"
      "\t-t <thread_count>\n\t\tUse <thread_count> threads when multiplying matrices\n"
      "\t-o\n\t\tUse OpenMP instead of pthreads\n"
      "\t-g\n\t\tGenerate matrix data instead of reading it from a file\n"
      "\t-t\n\t\tTime the ring matrix multiplication (Don't print result)\n",
      argv[0]);
}

int main(int argc, char * argv[]) {
  get_args(argc, argv); // Process arguments

  // Check matrix size
  if (N < 1) {
    printf("Matrix must be of at least size 1.\n");
    exit(1);
  }

  MPI_Init(&argc, &argv); // Initialize MPI
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  if (IS_ROOT) { // Perform sanity checks
    if (numprocs > N) {
      printf("numprocs cannot be larger than N.\n"); exit(1);
    }

    if (N % numprocs != 0) {
      printf("numprocs must evenly divide N.\n"); exit(1);
    }
  }

  N_P = N / numprocs;
  int col_blk_sz = N_P * N;

  // Allocate and/or populate column blocks
  double * A_blk = (double *) malloc(col_blk_sz * sizeof(double));
  double * B_blk = (double *) malloc(col_blk_sz * sizeof(double));
  double * C_blk = (double *) calloc(col_blk_sz, sizeof(double));
  populate_matrices(A_blk, B_blk);

  param_list = (PARAMS *) calloc(THREAD_COUNT, sizeof(PARAMS));

  if (!ASYNC) { // Create global A_temp for synchronous communication
    A_tmp = (double *) malloc(sizeof(double) * col_blk_sz);
  }

  double start, end, delta;
  if (TIME) { // Start timer
    start = MPI_Wtime();
  }

  // Run the ring matrix multiplication algorithm
  if (ASYNC) {
    async_ring_multiply(myid, A_blk, B_blk, C_blk);
  } else {
    sync_ring_multiply(myid, A_blk, B_blk, C_blk);
  }

  if (TIME) { // Print timing results
    end = MPI_Wtime();
    delta = end - start;

    // Reduce times
    double time_max, time_min, time_sum;
    MPI_Reduce(&delta, &time_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&delta, &time_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&delta, &time_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (IS_ROOT) { // Print results
      double time_avg = time_sum / numprocs;
      printf("avg: %.6lf min: %.6lf max: %.6lf "
        "async: %c pthread: %c t_count: %2d read_file: %c N: %3d P: %3d\n",
        time_avg, time_min, time_max,
        ASYNC ? 'T' : 'F',
        PTHREAD ? 'T' : 'F',
        THREAD_COUNT,
        READ_FILE ? 'T' : 'F',
        N, numprocs);
    }
  } else { // Print program parameters and resulting matrix
    double * C_result = NULL;
    if (IS_ROOT) {
      C_result = (double *) malloc(N * N * sizeof(double));
    }

    MPI_Gather(C_blk, col_blk_sz, MPI_DOUBLE,
      C_result, col_blk_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (IS_ROOT) { 
      printf("async: %c pthread: %c t_count: %2d read_file: %c N: %3d P: %3d\n",
        ASYNC ? 'T' : 'F',
        PTHREAD ? 'T' : 'F',
        THREAD_COUNT,
        READ_FILE ? 'T' : 'F',
        N, numprocs);
      print_matrix_array(NULL, C_result, N, N);
      free(C_result);
    }
  }

  // Deallocate resources
  free(A_blk);
  free(B_blk);
  free(C_blk);
  free(param_list);
  if (!ASYNC) { free(A_tmp); }

  MPI_Finalize(); // End MPI

  return 0;
}

void populate_matrices(double * A_blk, double * B_blk) {
  int col_blk_sz = N_P * N;
  if (READ_FILE) {
    double * A = NULL, * B = NULL;

    if (IS_ROOT) {
      A = malloc((N * N) * sizeof(double));
      B = malloc((N * N) * sizeof(double));
      read_matrix_file(matrix_a_file, A);
      read_matrix_file(matrix_b_file, B);
    }
    
    MPI_Scatter(A, col_blk_sz, MPI_DOUBLE, A_blk, col_blk_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, col_blk_sz, MPI_DOUBLE, B_blk, col_blk_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);  

    if (IS_ROOT) { free(A); free(B); }
  } else { // Set all array values to 1
    int i;
    for (i = 0; i < col_blk_sz; i++) {
      A_blk[i] = 1; B_blk[i] = 1;
    }
  }
}

void async_ring_multiply(int myid, double * A, double * B, double * C) {
  double * A_curr = A;
  double * A_next = (double *) malloc(N_P * N * sizeof(double));
  double * A_tmp = (double *) malloc(N_P * N * sizeof(double));
  MPI_Request send_req, recv_req;

  int blksz = N_P * N * sizeof(double);
  int left = (myid + numprocs - 1) % numprocs;
  int right = (myid + 1) % numprocs;

  int k = myid;
  int b_row_start = k * N_P;
  threaded_multiply(A, B, C, b_row_start);

  // Receive next from right
  MPI_Irecv(A_next, N * N_P, MPI_DOUBLE, right, 0, MPI_COMM_WORLD, &recv_req);
  // Copy curr into temp
  memcpy(A_tmp, A_curr, blksz);
  // Send temp to left
  MPI_Isend(A_tmp, N * N_P, MPI_DOUBLE, left, 0, MPI_COMM_WORLD, &send_req);
  
  int i;
  for (i = 1; i < numprocs; i++) {
    k = (k + 1) % numprocs;

    // Wait for recv request to complete
    MPI_Wait(&recv_req, MPI_STATUS_IGNORE);
    // Copy next into curr
    memcpy(A_curr, A_next, blksz);
    // Receive next from right
    MPI_Irecv(A_next, N * N_P, MPI_DOUBLE, right, 0, MPI_COMM_WORLD, &recv_req);
    // Wait for send request
    MPI_Wait(&send_req, MPI_STATUS_IGNORE);
    // Copy curr into temp
    memcpy(A_tmp, A_curr, blksz);
    // Send tempt to left
    MPI_Isend(A_tmp, N * N_P, MPI_DOUBLE, left, 0, MPI_COMM_WORLD, &send_req);

    b_row_start = k * N_P;
    threaded_multiply(A, B, C, b_row_start);
  }
}

void sync_ring_multiply(int myid, double * A, double * B, double * C) {
  int k = myid;
  int b_row_start = k * N_P;
  int left = (myid + numprocs - 1) % numprocs;
  int right = (myid + 1) % numprocs;

  threaded_multiply(A, B, C, b_row_start);

  int i;
  for (i = 1; i < numprocs; i++) {
    k = (k + 1) % numprocs;
    sync_transfer(A, left, right, myid);
    b_row_start = k * N_P;
    threaded_multiply(A, B, C, b_row_start);
  }
}

void sync_transfer(double * A, int left, int right, int myid) {
  MPI_Request send_req;

  MPI_Isend(A, N * N_P, MPI_DOUBLE, left, 0, MPI_COMM_WORLD, &send_req);
  MPI_Recv(A_tmp, N * N_P, MPI_DOUBLE, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  MPI_Wait(&send_req, MPI_STATUS_IGNORE); // Wait for send to complete
  memcpy(A, A_tmp, N_P * N * sizeof(double)); // Copy received block to A
}

void read_matrix_file(char * file_name, double * M) {
  FILE * matrix_file = fopen(file_name, "r");
  if (matrix_file == NULL) {
    perror("Could not open file.");
    exit(1);
  }
  int i, j, index;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      index = (N * j) + i; // Read by column
      fscanf(matrix_file, "%lf", &M[index]); // Read a number
    }
  }
  fclose(matrix_file);
}

void print_matrix_array(char * m_name, double * m_array, int rows, int columns) {
  if (m_name) {
    printf("%s:\n", m_name);
  }

  int i, j, index;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < columns; j++) {
      index = (rows * j) + i;
      printf("%.0lf%s", m_array[index],
        (j == columns - 1) ? "\n" : " ");
    }
  }
}

void threaded_multiply(double * A, double * B, double * C, int b_row_start) {
  int i;
  for (i = 0; i < THREAD_COUNT; i++) {
    populate_args(&param_list[i], A, B, C, b_row_start, i);
  }

  if (THREAD_COUNT == 1) { matrix_multiply(&param_list[0]); return; }

  if (PTHREAD) { // Do multiplication with pthreads
    pthread_t * tids = (pthread_t *) malloc(sizeof(pthread_t) * (THREAD_COUNT));
    for (i = 0; i < THREAD_COUNT; i++) { // Create threads
      pthread_create(&tids[i], NULL, &matrix_multiply, (void *) &param_list[i]);
    }
    for (i = 0; i < THREAD_COUNT; i++) { pthread_join(tids[i], NULL); }
  } else { // Do multiplication with OpenMP
    omp_set_dynamic(0); // Only allow set number of threads
    #pragma omp parallel num_threads(THREAD_COUNT) \
      default(none) shared(param_list) firstprivate(N, N_P)
      matrix_multiply((void *) &param_list[omp_get_thread_num()]);
  }
}

void * matrix_multiply(void * args) {
  // Get parameters
  double * A, * B, * C;
  int b_row_start, b_col_start, b_col_end, i;
  unpack_args(args, &A, &B, &C, &b_row_start, &b_col_start, &b_col_end);

  int size = b_col_end - b_col_start;
  double * C_local = (double *) calloc(N * size, sizeof(double));

  int a_col, a_row, b_col, b_row, a_idx, b_idx, c_idx;
  for (b_col = b_col_start; b_col < b_col_end; b_col++) { // Loop through all columns of B
    for (a_col = 0; a_col < N_P; a_col++) { // Loop through all columns of A
      b_row = a_col + b_row_start;
      b_idx = (N * b_col) + b_row;
      for (a_row = 0; a_row < N; a_row++) { // Loop through all rows of A
        c_idx = (N * (b_col - b_col_start)) + a_row;
        a_idx = (N * a_col) + a_row;
        C_local[c_idx] += A[a_idx] * B[b_idx]; // Multiply into C
      }
    }
  }
  // Add local C to global
  pthread_mutex_lock(&mutex);
  for(i = 0; i < N * size; i++) {
    C[i + b_col_start * N] += C_local[i];
  }
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void populate_args(PARAMS * args, double * A, double * B, double * C,
  int b_row_start, int rank) {
  // Calculate starting and ending columns
  int cols = N_P / THREAD_COUNT;
  int b_col_start = cols * rank;
  int b_col_end = b_col_start + cols;
  // Assign values to struct
  args->A = A;
  args->B = B;
  args->C = C;
  args->b_row_start = b_row_start;
  args->b_col_start = b_col_start;
  args->b_col_end = b_col_end;
}

void unpack_args(void * args, double ** A, double ** B, double ** C,
  int * b_row_start, int * b_col_start, int * b_col_end) {
  PARAMS * params = (PARAMS *) args;
  *A = params->A;
  *B = params->B;
  *C = params->C;
  *b_row_start = params->b_row_start;
  *b_col_start = params->b_col_start;
  *b_col_end = params->b_col_end;
}
