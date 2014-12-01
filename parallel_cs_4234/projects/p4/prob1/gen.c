#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

#define DATA_DIR "data"

void write_file(char * file_name, int * array);
void popluate_matrix(int * M);
void calculate_result(int * A, int * B, int * C);

int N;

int main(int argc, char ** argv) {
	if(argc != 2) {
		printf("Usage: %s <matrix_size>\n", argv[0]);
		exit(1);
	}

	N = strtol(argv[1], NULL, 10);
	if (N < 1) {
		perror("Matrix must be of at least size 1");
		exit(1);
	}
	
	srand(time(NULL));
	int * A = malloc((N * N) * sizeof(int));
	int * B = malloc((N * N) * sizeof(int));
	int * C = calloc(N * N, sizeof(int));

	popluate_matrix(A);
	popluate_matrix(B);

	calculate_result(A, B, C);

	if (mkdir(DATA_DIR, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) {
		if (errno != EEXIST) {
			perror("Could  not create directory");
			exit(1);
		}
	}

	char file_name[256];
	sprintf(file_name, "%s/matrix_%d_a.txt", DATA_DIR, N);
	write_file(file_name, A);
	
	sprintf(file_name, "%s/matrix_%d_b.txt", DATA_DIR, N);
	write_file(file_name, B);

	sprintf(file_name, "%s/matrix_%d_c.txt", DATA_DIR, N);
	write_file(file_name, C);
	
	free(A);
	free(B);
	free(C);
	return 1;
}

void write_file(char * file_name, int * array) {
	printf("Writing %dx%d matrix to: \"%s\"\n", N, N, file_name);
	FILE * file = fopen(file_name, "w");
	if (file == NULL) {
		perror("Could not create file");
		exit(1);
	}
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			int index = (N * i) + j;
			fprintf(file, "%d%s", array[index], (j == N - 1 ? "\n" : " "));
		}
	}
	fclose(file);
}

void popluate_matrix(int * M) {
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			int index = (N * i) + j;
			M[index] = rand() % 10;
		}
	}
}

void calculate_result(int * A, int * B, int * C) {
	int a_row, b_col, i;
	for (a_row = 0; a_row < N; a_row++) {
		for (b_col = 0; b_col < N; b_col++) {
			for (i = 0; i < N; i++) {
				int a_index = (N * a_row) + i;
				int b_index = (N * i) + b_col;
				int c_index = (N * a_row) + b_col;
				C[c_index] += A[a_index] * B[b_index]; 
			}
		}
	}
}
