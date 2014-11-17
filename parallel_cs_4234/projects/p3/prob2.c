#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

void get_args(int argc, char ** argv);

int consumers; // Number of consumer threads
int producers; // Number of producer threads
char ** input_files; // File names for input files

#define DEBUG

int main(int argc, char ** argv) {
	get_args(argc, argv);

	return 0;
}

void get_args(int argc, char ** argv) {
	if (argc < 3) {
		printf("Usage: <consumer threads> <input file> ...");
		exit(1);
	}
	consumers = strtol(argv[1], NULL, 10);
	producers = argc - 2;
	input_files = &argv[2];
	#ifdef DEBUG
	printf("Consumers: %d Producers: %d Files:\n", consumers, producers);
	int i;
	for (i = 0; i < producers; i++) {
		printf("[%d] %s\n", i, input_files[i]);
	}
	#endif
}
