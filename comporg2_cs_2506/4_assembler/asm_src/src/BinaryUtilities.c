/*
 * BinaryUtilities.c
 * 	This source file handles operations regarding the translation of string and
 * 	number to binary representation. 
 *
 *  Created on: Nov 14, 2013
 *      Author: Elmer Landaverde
 */
#include "BinaryUtilities.h"

bool write_to_bin(const char * const token, const Parameter_Info * const par,
		char * const bin_line) {
	// Check input
	if (token == NULL || par == NULL || bin_line == NULL) {
		printf("Error: One or more input pointers are null.\n");
		return false;
	}

	// Get start and ending position.
	uint32_t start = (31 - par->star_pos);
	uint32_t end = start + par->size;

	// Copy number to binary line.
	uint32_t i = start;
	uint32_t j = 0;
	while (i < end) {
		bin_line[i] = token[j];
		i++;
		j++;
	}
	return true;
}

bool write_num_to_bin(uint32_t number, const Parameter_Info * const par,
		char * const bin_line) {
	// Convert number to binary characters.
	char num_char[] = "00000";
	int j = 0;
	for (int i = 4; i >= 0; i--) {
		num_char[j] = (number & (1 << i)) ? '1' : '0';
		j++;
	}
	return write_to_bin(num_char, par, bin_line);
}

bool write_num_to_bin_16(uint16_t number, const Parameter_Info * const par,
		char * const bin_line) {
	// Convert number to binary characters.
	char num_char[] = "0000000000000000";
	int j = 0;
	for (int i = 15; i >= 0; i--) {
		num_char[j] = (number & (1 << i)) ? '1' : '0';
		j++;
	}
	return write_to_bin(num_char, par, bin_line);
}

bool write_num_to_bin_16_s(int16_t number, const Parameter_Info * const par,
		char * const bin_line) {
	// Convert number to binary characters.
	char num_char[] = "0000000000000000";
	int j = 0;
	for (int i = 15; i >= 0; i--) {
		num_char[j] = (number & (1 << i)) ? '1' : '0';
		j++;
	}
	return write_to_bin(num_char, par, bin_line);
}

bool write_register(const char * const reg, const Parameter_Info * const par,
		char * const line) {
	if (reg == NULL || par == NULL || line == NULL) {
		printf("Error: One or more input pointers are null.\n");
		return false;
	}
	// Get register index.
	int i = get_index(reg, REG_TBL, REG_TBL_SIZE);
	if (i == -1) {
		printf("Error: Register is unknown.\n");
		return false;
	}
	// Write register to line.
	if (!write_to_bin(REG_TBL[i].bin, par, line)) {
		printf("Error: Could not write register to line.\n");
		return false;
	}
	return true;
}

void get_bin_number(const char * const num, char * const bin_number) {
	int32_t number = parse_int_s(num);
	// Create number template.
	bin_number[WORD_LEN - 1] = '\0';
	int i = WORD_LEN - 2;
	while (i >= 0) {
		if (number & 1) {
			bin_number[i] = '1';
		} else {
			bin_number[i] = '0';
		}
		number >>= 1;
		i--;
	}
}

bool allocate_bin(Instruction_Info * const inst) {
	inst->binary = malloc(sizeof(char) * BIN_LN_LEN);
	if (inst->binary == NULL) {
		printf("Error: Could allocate memory for binary line.\n");
		return false;
	}
	for (int i = 0; i < BIN_LN_LEN - 1; i++) {
		inst->binary[i] = '\0';
	}
	return true;
}
