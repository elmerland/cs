#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

void printTag1A(uint16_t number);
void printTag1B(uint16_t number);

int main(void) {
	printf("Addr\tTag\t\tSet\tIdx\n");
	printf("-------------------------------------\n");
	uint16_t nums[12] = {
		21, 166, 229, 279, 61, 238, 190, 165, 239, 279, 60, 237
	};
	for (int i = 0; i < 12; i++) {
		printTag1B(nums[i]);
	}
	return 0;
}

void printTag1A(uint16_t number) {
	printf("%d\t\t", number);
	for (int i = 15; i >= 7; i--) {
		uint16_t mask = 1 << i;
		printf("%c", (number & mask) ? '1' : '0');
	}
	printf("\t");
	for (int i = 6; i >= 3; i--) {
		uint16_t mask = 1 << i;
		printf("%c", (number & mask) ? '1' : '0');
	}
	printf("\t");
	for (int i = 2; i >= 0; i--) {
		uint16_t mask = 1 << i;
		printf("%c", (number & mask) ? '1' : '0');
	}
	printf("\n");
}

void printTag1B(uint16_t number) {
	printf("%d\t\t", number);
	for (int i = 15; i >= 6; i--) {
		uint16_t mask = 1 << i;
		printf("%c", (number & mask) ? '1' : '0');
	}
	printf("\t");
	for (int i = 5; i >= 4; i--) {
		uint16_t mask = 1 << i;
		printf("%c", (number & mask) ? '1' : '0');
	}
	printf("\t");
	for (int i = 3; i >= 0; i--) {
		uint16_t mask = 1 << i;
		printf("%c", (number & mask) ? '1' : '0');
	}
	printf("\n");
}
