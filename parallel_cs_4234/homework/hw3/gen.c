#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double gen_random();

double max = 100.0;
double min = 0.0;

int main(int argc, char ** argv) {
	if (argc != 3) {
		printf("Invalid arguments\n");
		exit(1);
	}
	long m = strtol(argv[1], NULL, 10);
	long n = strtol(argv[2], NULL, 10);
	srand(time(NULL));

	printf("%ld %ld\n", m, n);
	int i, j;
	for (i = 0; i < n; i++) {
		printf("%lf ", gen_random());
	}
	printf("\n");
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			printf("%lf ", gen_random());
		}
		printf("\n");
	}
	return 0;
}

double gen_random() {
	return  (max - min) * ((double)rand())/((double)RAND_MAX) + min;
}