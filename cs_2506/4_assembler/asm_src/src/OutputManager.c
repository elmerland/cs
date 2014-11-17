/*
 * OutputManager.c
 * 	This source file is in charge of outputting to file.
 * 		-list mode
 * 			Prints a list of all the instructions accompanied with their address
 * 			and original instructions.
 * 			Prints a list of all the variables accompanied with their address.
 * 		-symbols mode
 * 			Prints a list of all the symbols (labels) in the assembler input
 * 			file. It also prints the address of each symbol.
 * 		-naked (binary) mode
 * 			Prints a binary representation of the assembler input file.
 *
 *  Created on: Oct 23, 2013
 *      Author: Elmer Landaverde
 */
#include "OutputManager.h"

/*
 *	print_symbols() This function prints the appropriate output for the symbols
 * 			option. It prints a header with two columns: address and symbols. It
 * 			then prints all the symbols in the input file that was assembled in 
 * 			ascending address order.
 *	Pre:	The output file pointer points to a writable file.
 *	Post:	The output file has a symbol table that corresponds to the input
 *			assembler file.
 */
static void print_symbols(FILE *output);

/*
 *	print_list() This function prints the appropriate output for the list
 *			option. It prints all the binary lines that are normally printed by
 *			not using any options. However the address of each instruction will
 *			be printed to the left of each line and the original assembly
 *			instruction will be printed to the right. If an instruction has a
 *			label, this label will be printed before the instruction on its own
 *			line. This is the same for variables except that variables do not
 *			have anything printed to the right of them.
 *	Pre:	The output file pointer points to a writable file.
 *	Post:	The output file has a list representation the corresponds to the
 *			input assembler file.
 */
static void print_list(FILE *output);

/*
 *	print_naked_binary() This function prints the binary representation of the
 *			assembly input file. Each line is 32 characters long followed by a
 *			new line character. The text and data section are separated by an
 *			empty line.
 *	Pre:	The output file points to a writable file.
 *	Post:	The output file contains the binary representation of the assembly
 *			input file. 
 */
static void print_naked_binary(FILE *output);

/*
 *	print_bin_int_array() Prints the binary representation of a integer array.
 *	Pre:	The output file pints to a writable file.
 *			Var points to a properly initialized variable struct that is of type
 *			WORD_A.
 *			The print_addr boolean determines if the address of each element in
 *			the array should be printed as well.
 *	Post:	The output file contains the binary representation of the specified
 *			integer array.
 */
static void print_bin_int_array(FILE *output, const Variable_Info * const var,
		bool print_addr);

/*
 *	print_instructions() Prints the binary representation of all the
 *			instructions in the instruction table.
 *	Pre:	The output file points to a writable file.
 *			The prnt_labels boolean determines if the address of each
 *			instruction should be printed.
 *	Post:	The output file contains the binary representation of all the
 *			instruction corresponding to the input file.
 */
static void print_instructions(FILE *output, bool prnt_labels);

/*
 *	print_variables() Prints a binary representation of all the variables in the
 *			variable table.
 *	Pre:	The output file  points to a writable file.
 *			The print_addr boolean determines if the address of each line in
 *			each variable should be printed.
 *	Post:	The output file contains the binary representation of all the
 *			variables corresponding to the input file.
 */
static void print_variables(FILE *output, bool print_addr);

/*
 *	print_bin_int_array_s() This function does the same as the
 *			print_bin_int_array function except that it handles all arrays of
 *			type WORD_A and WORD_A_S.
 *	Pre:	The output file points to a writable file.
 *			var points to a properly initialized variable struct.
 *			the print_addr boolean determines if the address of each element in
 *			the array should be printed.
 *	Post:	The output file contains the binary representation of the integer
 *			array.
 */
static void print_bin_int_array_s(FILE *output, const Variable_Info * const var,
		bool print_addr);

/*
 *	print_string() Prints a string representation of a string variable. It also
 *			prints any other strings that immediately follow this string until
 *			it gets to a non-string variable or the end of the variable table.
 *	Pre:	The output file points to a writable file.
 *			The index is the position of the string to be printed in the
 *			variables table.
 *			The print_addr boolean determines if the addresses should be
 *			printed.
 *	Post:	The output file contains the binary representation of the given
 *			strings.
 *	Return:	The index of the last variable that was printed by this function.
 */
static uint32_t print_string(FILE *output, uint32_t index, bool print_addr);

/*
 *	print_naked_string() Prints the binary representation of a string. This
 *			function first concatenates all string variables and then prints
 *			every single string in one block.
 *	Pre:	The output file points to a writable file.
 *			The index corresponds to the variable in the variable table that
 *			will be printed by this function.
 *	Post:	The output file will contain the binary representation of this and
 *			all the strings immediately following this string.
 *	Return:	The index of the last string that was printed to the output file.
 */
static uint32_t print_naked_string(FILE *output, uint32_t index);

/*
 *	print_list_string() Prints a binary representation of a string and prints
 *			the variable address to the left of each line. This function prints
 *			all the string following the current string along with the current
 *			string and prints the address of each line as well.
 *	Pre:	The output file points to a writable file.
 *			The index corresponds to the variable in the variable table that
 *			will be printed by this function.
 *	Post:	The output file will contain the binary representation of the
 *			current string and all the strings immediately following it.
 *	Return:	The index of the last string that was printed to file.
 */
static uint32_t print_list_string(FILE *output, uint32_t index);

void print_output(bool listOption, bool symbolsOption, FILE *output) {
	if (output == NULL) {
		printf("Error: Output file pointer is null.\n");
		return;
	}
	if (symbolsOption && !listOption) {
		print_symbols(output);
	} else if (listOption && !symbolsOption) {
		print_list(output);
	} else if (!listOption && !symbolsOption) {
		print_naked_binary(output);
	}
	rewind(output);
}

static void print_naked_binary(FILE *output) {
	if (sym_tbl == NULL) {
		printf("Error: Symbol table is null.\n");
		return;
	}
	if (inst_tbl == NULL) {
		printf("Error: Instructions table is null.\n");
		return;
	}
	if (var_tbl == NULL) {
		printf("Error: Variables table is null.\n");
		return;
	}
	print_instructions(output, false);

	fprintf(output, "\n");

	print_variables(output, false);
}

static void print_symbols(FILE *output) {
	if (sym_tbl == NULL) {
		printf("Error: Symbol table is null.\n");
		return;
	}
	char *header = "Address\t\tSymbol\n";
	char *divider = "-----------------------------------\n";
	fprintf(output, "%s", header);
	fprintf(output, "%s", divider);
	// Print symbols
	for (uint32_t i = 0; i < sym_cnt; i++) {
		fprintf(output, "0x%.8"PRIX32"\t%s\n", sym_tbl[i].addr,
				sym_tbl[i].symbol);
	}
}

static void print_list(FILE *output) {
	if (sym_tbl == NULL) {
		printf("Error: Symbol table is null.\n");
		return;
	}
	if (inst_tbl == NULL) {
		printf("Error: Instructions table is null.\n");
		return;
	}
	if (var_tbl == NULL) {
		printf("Error: Variables table is null.\n");
		return;
	}
	print_instructions(output, true);

	fprintf(output, "\n");

	print_variables(output, true);
}

static void print_instructions(FILE *output, bool prnt_labels) {
	for (uint32_t i = 0; i < inst_cnt; i++) {
		if (prnt_labels) {
			print_label(inst_tbl[i].addr, output, true);
		}

		if (prnt_labels) {
			fprintf(output, "0x%.8"PRIX32"\t%s\t%s", inst_tbl[i].addr,
					inst_tbl[i].binary, inst_tbl[i].line);
		} else {
			fprintf(output, "%s\n", inst_tbl[i].binary);
		}
	}
}

static void print_variables(FILE *output, bool print_addr) {
	for (uint32_t i = 0; i < var_cnt; i++) {
		if (print_addr) {
			print_label(var_tbl[i].addr, output, false);
		}

		if (var_tbl[i].type == WORD_A) {
			// Print integer array.
			print_bin_int_array(output, &var_tbl[i], print_addr);
		} else if (var_tbl[i].type == WORD_A_S || var_tbl[i].type == WORD) {
			// Print set integer array.
			print_bin_int_array_s(output, &var_tbl[i], print_addr);
		} else if (var_tbl[i].type == ASCII) {
			// Print string.
			i = print_string(output, i, print_addr);
		}
	}
}

static void print_bin_int_array(FILE *output, const Variable_Info * const var,
		bool print_addr) {
	if (var->type == ASCII || var->type == V_NOT_DEF) {
		printf("Error: Variable type is not a word or word array.\n");
		return;
	}
	// Get array size.
	uint32_t size = var->length;

	// Create number template.
	char bin_number[WORD_LEN];
	get_bin_number(var->parameter, bin_number);

	// Print binary.
	uint32_t addr = var->addr;
	for (uint32_t j = 0; j < size; j++) {
		if (print_addr) {
			fprintf(output, "0x%.8"PRIX32"\t%s\n", addr, bin_number);
		} else {
			fprintf(output, "%s\n", bin_number);
		}
		addr += 4;
	}
}

static void print_bin_int_array_s(FILE *output, const Variable_Info * const var,
		bool print_addr) {
	uint32_t addr = var->addr;
	char param[TKN_LEN];
	strncpy(param, var->parameter, TKN_LEN);
	char *num = strtok(param, VAR_A_S_DELIMETER);
	char bin_number[WORD_LEN];
	while (num != NULL && strlen(num) > 0) {
		get_bin_number(num, bin_number);
		if (print_addr) {
			fprintf(output, "0x%.8"PRIX32"\t%s\n", addr, bin_number);
			addr += 4;
		} else {
			fprintf(output, "%s\n", bin_number);
		}
		num = strtok(NULL, VAR_A_S_DELIMETER);
	}
}

static uint32_t print_string(FILE *output, uint32_t index, bool print_addr) {
	if (!print_addr) {
		return print_naked_string(output, index);
	} else {
		return print_list_string(output, index);
	}
}

static uint32_t print_naked_string(FILE *output, uint32_t index) {
	// Get length of all strings.
	uint32_t len = 0;
	uint32_t last_string = index;
	for (uint32_t i = index; i < var_cnt; i++) {
		if (var_tbl[i].type == ASCII) {
			len += var_tbl[i].length;
			last_string = i;
		} else {
			break;
		}
	}
	// Concatenate all strings.
	char str[len];
	uint32_t str_indx = 0;
	for (uint32_t i = index; i < var_cnt; i++) {
		if (var_tbl[i].type != ASCII) {
			break;
		}
		char *temp_str = var_tbl[i].parameter;
		for (uint32_t j = 0; temp_str[j] != '\0'; j++) {
			str[str_indx] = temp_str[j];
			str_indx++;
		}
		str[str_indx] = '\0';
		str_indx++;
	}

	// Print string to file.
	uint32_t print_size = len - (len % 4);
	uint32_t remaining = len - print_size;

	// Print main character block
	for (uint32_t i = 0; i < print_size; i += 4) {
		print_bin_chars(output, str[i], str[i + 1], str[i + 2], str[i + 3]);
	}

	// Check if there are any remaining characters.
	if (remaining == 0) {
		return last_string;
	}
	// Print remaining chars.
	for (uint32_t i = 0; i < (4 - remaining); i++) {
		print_bin_char(output, '\0');
	}
	str_indx = len - 1;
	for (uint32_t i = remaining; i > 0; i--) {
		print_bin_char(output, str[str_indx]);
		str_indx--;
	}
	fprintf(output, "\n");
	return last_string;
}

static uint32_t print_list_string(FILE *output, uint32_t index) {
	char *str = var_tbl[index].parameter;
	uint32_t start_addr = var_tbl[index].addr;
	uint32_t len = var_tbl[index].length;
	uint32_t str_indx = 0;

	// Initialize buffer
	char buffer[4];
	buffer[0] = '\0';
	buffer[1] = '\0';
	buffer[2] = '\0';
	buffer[3] = '\0';
	uint32_t buffer_pos = start_addr % 4;
	
	// Print string.
	uint32_t curr_addr = start_addr - (start_addr%4);
	while (str_indx < len) {
		if (str_indx == 0) {
			print_address(output, start_addr);
		}
		else {
			print_address(output, curr_addr);
		}
		// Fill buffer
		while (buffer_pos < 4 && str_indx < len) {
			buffer[buffer_pos] = str[str_indx];
			buffer_pos++;
			str_indx++;
		}
		// Print buffer
		print_bin_chars(output, buffer[0], buffer[1], buffer[2], buffer[3]);
		buffer_pos = 0;
		buffer[0] = '\0';
		buffer[1] = '\0';
		buffer[2] = '\0';
		buffer[3] = '\0';
		
		curr_addr += 4;
	}
	return index++;
}
