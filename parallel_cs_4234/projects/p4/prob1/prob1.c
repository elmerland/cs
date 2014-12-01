#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

void read_matrix_file(char * file_name, double * M);
void print_matrix_array(char * m_name, double * m_array, int rows, int columns);
void ring_multiply(int myid, double * A, double * B, double * C);
void multiply_matrix(double * A, double * B, double * C, int b_row_start);

int N; // Size of matrix
int N_P; // Size of sub-blocks
int numprocs; // Number of processes

int main(int argc, char * argv[]) {
  int myid;

  if (argc != 4) {
    printf("Usage: %s <matrix_size> <input_file_A> <input_file_B>\n", argv[0]);
    exit(1);
  }

  N = strtol(argv[1], NULL, 10);

  if (N < 1) {
    printf("Matrix must be of at least size 1.\n");
    exit(1);
  }

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  if (myid == 0) {
    if (numprocs > N) {
      printf("numprocs cannot be larger than N.\n");
      exit(1);
    } else if (N % numprocs != 0) {
      printf("numprocs must evenly divide N.\n");
      exit(1);
    }
  }
  N_P = N / numprocs;
  int col_blk_cols = N_P;
  int col_blk_sz = col_blk_cols * N;

  if (myid == 0) {
    // printf("Testing with %d processes:\n", numprocs);

    double * A = malloc((N * N) * sizeof(double));
    double * B = malloc((N * N) * sizeof(double));

    read_matrix_file(argv[2], A);
    read_matrix_file(argv[3], B);

    // print_matrix_array("Matrix A", A, N, N);
    // print_matrix_array("Matrix B", B, N, N);

    MPI_Barrier(MPI_COMM_WORLD);

    int i;
    for (i = 0; i < numprocs; i++) {
      double * A_col_blk = A + (i * col_blk_sz);
      double * B_col_blk = B + (i * col_blk_sz);
      MPI_Send(A_col_blk, col_blk_sz, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      MPI_Send(B_col_blk, col_blk_sz, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
    }
    free(A);
    free(B);
  } else {
    MPI_Barrier(MPI_COMM_WORLD);
  }

  double * A_blk = (double *) malloc(col_blk_sz * sizeof(double));
  double * B_blk = (double *) malloc(col_blk_sz * sizeof(double));
  double * C_blk = (double *) calloc(col_blk_sz, sizeof(double));

  // Receive column block for A
  MPI_Recv(A_blk, col_blk_sz, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, NULL);
  // Receive column block for B
  MPI_Recv(B_blk, col_blk_sz, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, NULL);

  // char mat_name[50];
  // sprintf(mat_name, "Column Block A for Process %d", myid);
  // print_matrix_array(mat_name, A_blk, N, col_blk_cols);

  // sprintf(mat_name, "Column Block B for Process %d", myid);
  // print_matrix_array(mat_name, B_blk, N, col_blk_cols);

  ring_multiply(myid, A_blk, B_blk, C_blk);

  // sprintf(mat_name, "Column Block C for Process %d", myid);
  // print_matrix_array(mat_name, C_blk, N, col_blk_cols);

  double * C_result = NULL;
  if (myid == 0) {
    C_result = (double *) malloc(N * N * sizeof(double));
  }

  MPI_Gather(C_blk, col_blk_sz, MPI_DOUBLE,
    C_result, col_blk_sz, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (myid == 0) { 
    print_matrix_array(NULL, C_result, N, N);
    free(C_result);
  }

  free(A_blk);
  free(B_blk);
  free(C_blk);

  MPI_Finalize();

  return 0;
}

void ring_multiply(int myid, double * A, double * B, double * C) {
  int k = myid;
  int b_row_start = k * N_P;
  multiply_matrix(A, B, C, b_row_start);

  int i;
  for (i = 1; i < numprocs; i++) {
    k = (k + 1) % numprocs;
    int left = (myid + numprocs - 1) % numprocs, right = (myid + 1) % numprocs;
    MPI_Send(A, N * N_P, MPI_DOUBLE, left, 0, MPI_COMM_WORLD);
    MPI_Recv(A, N * N_P, MPI_DOUBLE, right, 0, MPI_COMM_WORLD, NULL);
    b_row_start = k * N_P;
    multiply_matrix(A, B, C, b_row_start);
  }
}

void multiply_matrix(double * A, double * B, double * C, int b_row_start) {
  int a_col, a_row, b_col, b_row;
  for (a_col = 0; a_col < N_P; a_col++) { // Loop through all columns of A
    for (b_col = 0; b_col < N_P; b_col++) { // Loop through all columns of B
      b_row = a_col + b_row_start; // Get row of B
      int b_idx = (N_P * b_col) + b_row;

      for (a_row = 0; a_row < N; a_row++) { // Loop through all rows of A
        int c_idx = (N * b_col) + a_row;
        int a_idx = (N * a_col) + a_row;
        C[c_idx] += A[a_idx] * B[b_idx];
      }
    }
  }
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
      fscanf(matrix_file, "%lf", &M[index]); // Read a double
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





































