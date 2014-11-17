#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** argv) {
	int tc = strtol(argv[1], NULL, 10);
	// omp_set_num_threads(tc);

	#pragma omp parallel num_threads(tc)
	{
		int id = omp_get_thread_num();
		char buf[256];
		sprintf(buf, "Hello world form thread %d", id);
		puts(buf);
	}

	return 0;
}
