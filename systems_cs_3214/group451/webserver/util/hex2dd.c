
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** argv) {
	uint32_t num = (uint32_t)strtol(argv[1], NULL, 0);
	uint32_t mask = 0x000000FF;
	uint32_t d1 = (num >> 24) & mask;
	uint32_t d2 = (num >> 16) & mask;
	uint32_t d3 = (num >> 8) & mask;
	uint32_t d4 = (num) & mask;

	printf("%d.%d.%d.%d\n", d1, d2, d3, d4);
	return 0;
}
