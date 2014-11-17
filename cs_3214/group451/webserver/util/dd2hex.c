
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char ** argv) {

	char * str = argv[1];
	uint32_t hex = 0;
	uint32_t shift = 24;
	char * token = strtok(str, ".");
	while (token) {
		printf("%s\n", token);
		hex = ((uint32_t)strtol(token, NULL, 10) << shift) | hex;
		shift -= 8;
		token = strtok(NULL, ".");
	}

	printf("0x%X\n", hex);
	return 0;
}
