#include <stdio.h>

int AddEm(int *A, int Sz);

int main() {
	int Sz = 5;
	int A[5] = {32, 16, 64, 4, 8};

	int Sum = AddEm(A, Sz);
	printf("Sum: %d\n", Sum);
}

int AddEm(int *A, int Sz) {
	int v0 = 0;
	int *v1 = 0;
	v1 = A + (Sz * 4);
	while (A < v1) {
		v0 += *A;
		A = ((int)A) + 1;
	}
	return v0;
}
