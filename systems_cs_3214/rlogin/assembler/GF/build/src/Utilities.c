/*
 * Utilities.c
 * 	This source file contains several miscellaneous functions that are used
 * 	accross several source files.
 *
 *  Created on: Nov 7, 2013
 *      Author: Elmer Landaverde
 */

#include "Utilities.h"

bool has_error = false;

/**
 *	parse_digit() This function takes in one character and converts it to a
 *				uint32_t type. The character must be one of the digits from 0-9.
 *	Pre:		Digit contains a valid digit character.
 *	Post:		Digit remains unchanged.
 *	Returns:	The integer value of the character digit.
 */
static uint32_t parse_digit(const char const digit);

static uint32_t parse_digit(const char digit) {
	switch (digit) {
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	default:
		printf("Error: Character is not a digit.\n");
		return 0;
	}
}

uint32_t parse_int(const char * const number) {
	uint32_t result = 0;
	uint32_t j = 1;
	int size = strlen(number);
	for (int i = size - 1; i >= 0; i--) {
		result += j * parse_digit(number[i]);
		j *= 10;
	}
	return result;
}

int32_t parse_int_s(const char * const number) {
	bool is_neg = number[0] == '-';
	if (!is_neg) {
		return parse_int(number);
	}
	int32_t result = 0;
	int32_t j = 1;
	int size = strlen(number);
	for (int i = size - 1; i > 0; i--) {
		result += j * parse_digit(number[i]);
		j *= 10;
	}
	return result * -1;
}

uint16_t parse_int_16(const char * const number) {
	uint16_t result = 0;
	uint16_t j = 1;
	int size = strlen(number);
	for (int i = size - 1; i >= 0; i--) {
		result += j * parse_digit(number[i]);
		j *= 10;
	}
	return result;
}

int16_t parse_int_s_16(const char * const number) {
	bool is_neg = number[0] == '-';
	if (!is_neg) {
		return parse_int_16(number);
	}
	int16_t result = 0;
	int16_t j = 1;
	int size = strlen(number);
	for (int i = size - 1; i > 0; i--) {
		result += j * parse_digit(number[i]);
		j *= 10;
	}
	return result * -1;
}

void print_bin_chars(FILE *output, const char c1, const char c2, const char c3,
		const char c4) {
	print_bin_char(output, c4);
	print_bin_char(output, c3);
	print_bin_char(output, c2);
	print_bin_char(output, c1);
	fprintf(output, "\n");
}

void print_bin_char(FILE *output, char c) {
	for (int i = 7; i >= 0; i--) {
		fputc((c & (1 << i)) ? '1' : '0', output);
	}
}

void print_label(uint32_t addr, FILE *output, bool add_tabs) {
	for (uint32_t i = 0; i < sym_cnt; i++) {
		if (sym_tbl[i].addr == addr) {
			if (add_tabs) {
				fprintf(output, "\t\t\t\t\t\t\t\t\t\t\t\t");
			}
			fprintf(output, "%s:\n", sym_tbl[i].symbol);
			return;
		}
	}
}

void print_address(FILE *output, uint32_t addr) {
	fprintf(output, "0x%.8"PRIX32"\t", addr);
}

bool is_symbol(const char * const symbol) {
	if (symbol == NULL) {
		printf("Error: The pointer to the symbol is null.\n");
		return false;
	}
	int i = 0;
	while (symbol[i] != '\0') {
		if (i == 0 && isdigit(symbol[i])) {
			// Check if first character is a digit.
			return false;
		} else if (!(isalpha(symbol[i]) || symbol[i] == '_'
				|| isdigit(symbol[i]))) {
			// Check if character is a number, letter, or underscore.
			return false;
		}
		i++;
	}
	return true;
}

uint32_t get_symbol_addr(const char * const symbol) {
	for (uint32_t i = 0; i < sym_cnt; i++) {
		if (strncmp(symbol, sym_tbl[i].symbol, LABEL_LEN) == 0) {
			has_error = false;
			return sym_tbl[i].addr;
		}
	}
	printf("Error: No matching symbols where found.\n");
	has_error = true;
	return 0;
}

int32_t get_index(const char * const name, const Dictionary table[],
		uint32_t size) {
	if (name == NULL) {
		printf("Error: The pointer to the instruction name is null.\n");
		return -1;
	}
	for (uint32_t i = 0; i < size; i++) {
		if (strncmp(name, table[i].name, TKN_LEN) == 0) {
			return i;
		}
	}
	return -1;
}

bool contains(const char * const name, const char * const list[], int size) {
	if (name == NULL || list == NULL) {
		printf("Error: One or more pointer are null.\n");
		return false;
	}
	for (int i = 0; i < size; i++) {
		if (strncmp(name, list[i], TKN_LEN) == 0) {
			return true;
		}
	}
	return false;
}

void prnt_txt_file(FILE *file) {
	if (file == NULL) {
		printf("Error: The pointer to the text file is null.\n");
		return;
	}

	// Create string to hold each line.
	char curr_line[MAX_LN_LEN];
	// Get every line and print to console.
	int counter = 0;
	while (fgets(curr_line, MAX_LN_LEN, file) != NULL) {
		printf("[%.3d]>", counter);
		printf("%s", curr_line);
		counter++;
	}
	// Set file pointer to start of file.
	rewind(file);
}

bool has_lbl(const char * const line) {
	if (line == NULL) {
		printf("Error: The pointer to line is null.\n");
		return false;
	}

	for (int i = 0; line[i] != '\0'; i++) {
		if (line[i] == ':') {
			return true;
		} else if (line[i] == ' ' || line[i] == '.' || line[i] == '\t'
				|| line[i] == '\n') {
			return false;
		}
	}
	return false;
}

bool clean_lbl(char *line) {
	if (line == NULL) {
		printf("Error: Pointer to line is null.\n");
		return false;
	}
	char temp_label[MAX_LN_LEN];
	for (int i = 0; line[i] != '\0'; i++) {
		if (line[i] != ':') {
			temp_label[i] = line[i];
		} else {
			temp_label[i] = '\0';
		}
	}
	strcpy(line, temp_label);
	return true;
}
