/*
 * Preprocessor.c
 * 	This source file is in charge of getting rid of all non-essential
 * 	information in the input file. It deletes any leading or trailing spaces,
 * 	tabs or blank lines. It then separates the labels from the line of code by
 * 	putting the label in a line by itself.
 *
 *  Created on: Oct 20, 2013
 *      Author: Elmer Landaverde
 */

#include "Preprocessor.h"

/**
 *	Clean_Line() This function takes in a string array and removes all irrelevant
 *				characters. Irrelevant characters mean leading or trailing
 *				spaces, tabs, or commas; and anything after and including a '#'
 *				character. Each line will be terminated by a new line character
 *				followed by a null terminator. The maximum length of a line is
 *				256 bytes (not including the null terminator). If the line is
 *				larger than that undefined behavior will occur.
 *	Pre:		line points to a properly initialized string array and has a
 *				maximum length of 256 bytes (not including the null terminator).
 *	Post:		line contains only relevant characters and has a maximum length
 *				of 256 bytes (not including the null terminator).
 *	Returns:	True if the line could be cleaned successfully, false if line is
 *				a null pointer.
 */
static bool clean_ln(char *line);

/**
 *	split_ln() This functions separates the label from the rest of the assembly
 *				instruction. It allocates memory to a new string of length
 *				LABEL_LEN and returns it, so do not forget to free the string
 *				after it is no longer needed.
 *	Pre:		Lines points to a valid line of assembly code that has a label.
 *	Post:		The line string will be truncated to delete the label.
 *	Returns:	A pointer to a string that contains the label (Without the ':').
 */
static char* split_ln(char * const line);

/**
 *	Is_Sudo() This function determines if the given line of assembly code
 *				represents a sudo instruction. If the assembly line is indeed a
 *				sudo instruction, this function will modify the given line of
 *				code to reflect the actual instruction. If the sudo
 *				instruction needs to be translated into two lines of code then
 *				the function will return a pointer to the second line of code.
 *				If no second line is necessary, then it returns a NULL pointer
 *				while still modifying the given line of assembly.
 *	Pre:		Line contains a valid assembly line of code.
 *	Post:		If line is a sudo instruction it will be modified to reflect
 *				the real instruction.
 *				If the sudo instruction requires two lines of code it will
 *				return a pointer to string containing the value for the second
 *				instruction. If no second line is needed, it will return a NULL
 *				pointer.
 *	Returns:	If the sudo instruction requires two lines of code it will
 *				return a pointer to string containing the value for the second
 *				instruction. If no second line is needed, it will return a NULL
 *				pointer.
 */
static char* is_sudo(char *line);

/**
 *	Translate_Sudo_Inst() This function takes a Sudo_Instruction structure and
 *				constructs the replacement instruction. If the sudo instruction
 *				translates to two instruction then the function will return the
 *				value of the second line. Otherwise it will return null.
 *	Pre:		Line points to a valid assembly sudo instruction.
 *				Name points to a string array containing the name of the
 *				instruction.
 *				Par1 points to a string array containing the first parameter.
 *				Par2 points to a string array containing the second parameter.
 *				Par3 points to a string array containing the third parameter.
 *				Set to NULL if there is no third parameter.
 *	Post:		Line contains the correct non-sudo instruction.
 *				Sudo will not be changed.
 *	Returns:	If the sudo instruction requires a second line, then it will
 *				return that second line. Otherwise it returns NULL.
 */
static char* trans_sudo_inst(char *line, const char * const name,
		const char * const par1, const char * const par2,
		const char * const par3);

/**
 * gen_ln() This function generates a line of valid assembly code from the given
 * 				parameters.
 * 	Pre:		New line points to a MAX_LN_LEN size string that will hold the
 * 				new valid assembly line.
 * 				Name, the name of the function.
 * 				Par1, the first parameter.
 * 				Par2, the second parameter.
 * 				Par3, the third parameter (Set to NULL if there is none).
 * 	Post:		Everything except new_line remain unchanged. The new_line
 * 				contains the new assembly line.
 * 	Returns:	True if the new line was generated successfully. Otherwise, it
 * 				returns false.
 */
static bool gen_ln(char *new_line, const char * const name,
		const char * const par1, const char * const par2,
		const char const *par3);

/*
 * Array that holds all the possible sudo instructions.
 */
const char * const SUDO[] = { "la", "li", "blt", "ble", "nop" };

/*
 * Indexes for each of the sudo instructions. These constants are just to help
 * the code be more human readable.
 */
const uint32_t LA_INDEX = 0;
const uint32_t LI_INDEX = 1;
const uint32_t BLT_INDEX = 2;
const uint32_t BLE_INDEX = 3;
const uint32_t NOP_INDEX = 4;

/*
 * Total number of sudo instructions.
 */
const uint32_t SUDO_COUNT = 5;

uint32_t preprocess(FILE *input, FILE *output) {
	// Check inputs
	if (input == NULL || output == NULL) {
		printf("Error: One or more of the file pointers are null.\n");
		return -1;
	}

	// String array to hold the current line.
	char curr_line[MAX_LN_LEN];
	// Counter for number of non-irrelevant lines.
	uint32_t line_count = 0;

	// Read input file and write only relevant content to output file
	while (fgets(curr_line, MAX_LN_LEN, input) != NULL) {
		// Clean current line
		bool cleaned = clean_ln(curr_line);
		if (!cleaned) {
			printf("Error: Line [%"PRId32"] could not be cleaned\n",
					line_count);
			return -1;
		}
		// Determine if current line has a label.
		char *label_line = NULL;
		if (has_lbl(curr_line)) {
			// Remove label from line.
			label_line = split_ln(curr_line);
			if (label_line == NULL) {
				printf("Error: Label could not be split.\n");
				return -1;
			}
			// Write label line to output file.
			//printf("%s", label_line);
			fprintf(output, "%s", label_line);
			line_count++;
			free(label_line);
		}

		// Check if line is empty.
		if (curr_line[0] == '\n') {
			continue;
		}

		// Get sudo instruction
		char *extra_line = is_sudo(curr_line);

		// Check if line starts with a space.
		if (curr_line[0] == ' ')
			printf("Error: This is an empty line\n");
		// Print line to output file.
		//printf("%s", curr_line);
		fprintf(output, "%s", curr_line);
		line_count++;

		// Add extra line if necessary.
		if (extra_line != NULL) {
			//printf("%s", extra_line);
			fprintf(output, "%s", extra_line);
			free(extra_line);
			line_count++;
		}
	}

	// Set file pointer to start of files.
	rewind(input);
	rewind(output);
	// Return number of lines that were written to the output file.
	return line_count;
}

static char* split_ln(char * const line) {
	// Check input
	if (line == NULL) {
		printf("Error: The pointer to line string is null.\n");
		return NULL;
	}
	// Copy label.
	char *label = malloc(sizeof(char) * LABEL_LEN);
	for (int i = 0; line[i] != '\0'; i++) {
		if (line[i] != ':') {
			label[i] = line[i];
		} else {
			label[i] = line[i];
			label[i + 1] = '\n';
			label[i + 2] = '\0';
			break;
		}
	}
	// Remove label from original string.
	int j = 0;
	bool start_copying = false;
	for (int i = 0; line[i] != '\0'; i++) {
		if (line[i] == ':' && !start_copying) {
			start_copying = true;
		} else if (start_copying) {
			line[j] = line[i];
			j++;
		}
	}
	bool cleaned = clean_ln(line);
	if (!cleaned) {
		printf("Error: Line could not be cleaned.");
		return NULL;
	}
	return label;
}

static bool clean_ln(char *line) {
	// Check input.
	if (line == NULL) {
		printf("Error: The pointer to line string is null.\n");
		return false;
	}

	char temp_line[MAX_LN_LEN];
	bool has_started = false;
	bool in_quotes = false;
	bool in_token = false;
	int i = 0;
	int j = 0;
	while (i < MAX_LN_LEN && j < MAX_LN_LEN) {
		if ((line[i] == ' ' || line[i] == '\t') && !in_quotes) {
			// Character is a space, tab, or comma and its not inside quotes.
			if (in_token) {
				// This is the first occurrence of a space, tab or comma after
				// a word.
				in_token = false;
			}
			i++;
		} else if (line[i] == '#' && !in_quotes) {
			// Character is a sharp sign and its not inside quotes, which marks
			// the end of the line. Everything after this char is comments.
			temp_line[j] = '\n';
			temp_line[++j] = '\0';
			break;
		} else if (line[i] == '\n' && !in_quotes) {
			// Character is a new line and its not inside quotes, which marks
			// the end of the line.
			temp_line[j] = '\n';
			temp_line[++j] = '\0';
			break;
		} else {
			// The character is a relevant and valid character.
			if (line[i] == '\"') {
				// Next character is a quote sign.
				if (!in_quotes) {
					// If not in quotes, set in_quotes to true.
					in_quotes = true;
				} else {
					// If already in quotes, set in_quotes to false.
					in_quotes = false;
				}
			}

			if (!in_token) {
				if (has_started) {
					// If the last character was a space, or tab then add
					// a token separator to temp_line and proceed.
					temp_line[j++] = TOKEN_SEPARATOR;
				}
				// Set in_token to true, to indicate the start of a new token.
				in_token = true;
			}

			temp_line[j++] = line[i++];

			// Indicate that at least one valid character has being read for
			// this line.
			if (!has_started)
				has_started = true;
		}
	}

	// If the last character in temp_line is not a null terminator, then add it.
	if (temp_line[j] != '\0') {
		temp_line[j] = '\0';
	}

	// Copy the temp_line back to the original line.
	strcpy(line, temp_line);
	return true;
}

static char* is_sudo(char *line) {
	// Check input
	if (line == NULL) {
		printf("Error: Pointer to line is null.\n");
		return NULL;
	}
	// Create temporary string to tokenize.
	char temp[MAX_LN_LEN];
	strncpy(temp, line, MAX_LN_LEN);
	// Remove new line character at end of line.
	for (int i = 0; temp[i] != '\0'; i++) {
		if (temp[i] == '\n') {
			temp[i] = '\0';
			break;
		}
	}

	// Get instruction name.
	char* inst_name = strtok(temp, DELIMETER);
	if (inst_name == NULL) {
		printf("Error: Could not get instruction name.\n");
		return NULL;
	}
	// Determine if instruction is a sudo instruction.
	bool is_sudo = false;
	for (uint32_t i = 0; i < SUDO_COUNT; i++) {
		if (strcmp(inst_name, SUDO[i]) == 0) {
			is_sudo = true;
		}
	}
	if (!is_sudo) {
		return NULL;
	}

	// Get the parameters of the instruction.
	char *par1 = strtok(NULL, DELIMETER);
	char *par2 = strtok(NULL, DELIMETER);
	char *par3 = strtok(NULL, DELIMETER);

	return trans_sudo_inst(line, inst_name, par1, par2, par3);
}

static char* trans_sudo_inst(char *line, const char * const inst,
		const char * const par1, const char * const par2,
		const char * const par3) {
	if (strcmp(inst, SUDO[LA_INDEX]) == 0) {
		char new_line[MAX_LN_LEN];
		char *name = "addi";
		char *zero = "$zero";
		bool success = gen_ln(new_line, name, par1, zero, par2);
		if (!success) {
			printf("Error: could not generate sudo instruction.\n");
			return NULL;
		}
		strncpy(line, new_line, MAX_LN_LEN);
		return NULL;
	} else if (strcmp(inst, SUDO[LI_INDEX]) == 0) {
		char new_line[MAX_LN_LEN];
		char *name = "addiu";
		char *zero = "$zero";
		bool success = gen_ln(new_line, name, par1, zero, par2);
		if (!success) {
			printf("Error: could not generate sudo instruction.\n");
			return NULL;
		}
		strncpy(line, new_line, MAX_LN_LEN);
		return NULL;
	} else if (strcmp(inst, SUDO[NOP_INDEX]) == 0) {
		char new_line[MAX_LN_LEN];
		char *name = "sll";
		char *zero = "$zero";
		char *zeroi = "0";
		bool success = gen_ln(new_line, name, zero, zero, zeroi);
		if (!success) {
			printf("Error: could not generate sudo instruction.\n");
			return NULL;
		}
		strncpy(line, new_line, MAX_LN_LEN);
		return NULL;
	} else if (strcmp(inst, SUDO[BLT_INDEX]) == 0) {
		char new_line[MAX_LN_LEN];
		char new_line2[MAX_LN_LEN];
		char *name = "slt";
		char *name2 = "bne";
		char *at = "$at";
		char *zero = "$zero";
		bool success = gen_ln(new_line, name, at, par1, par2);
		if (!success) {
			printf("Error: could not generate sudo instruction.\n");
			return NULL;
		}
		success = gen_ln(new_line2, name2, at, zero, par3);
		if (!success) {
			printf("Error: could not generate sudo instruction.\n");
			return NULL;
		}
		strncpy(line, new_line, MAX_LN_LEN);
		char *second_line = malloc(sizeof(char) * MAX_LN_LEN);
		strncpy(second_line, new_line2, MAX_LN_LEN);
		return second_line;
	} else if (strcmp(inst, SUDO[BLE_INDEX]) == 0) {
		char new_line[MAX_LN_LEN];
		char new_line2[MAX_LN_LEN];
		char *name = "slt";
		char *name2 = "beq";
		char *at = "$at";
		char *zero = "$zero";
		bool success = gen_ln(new_line, name, at, par2, par1);
		if (!success) {
			printf("Error: could not generate sudo instruction.\n");
			return NULL;
		}
		success = gen_ln(new_line2, name2, at, zero, par3);
		if (!success) {
			printf("Error: could not generate sudo instruction.\n");
			return NULL;
		}
		strncpy(line, new_line, MAX_LN_LEN);
		char *second_line = malloc(sizeof(char) * MAX_LN_LEN);
		strncpy(second_line, new_line2, MAX_LN_LEN);
		return second_line;
	} else {
		printf("Error: Instruction name is not a sudo instruction.\n");
		return NULL;
	}
}

static bool gen_ln(char *new_line, const char * const name,
		const char * const par1, const char * const par2,
		const char * const par3) {
	// Get number of parameters.
	int par_num = 0;
	if (par1 != NULL) {
		par_num++;
	}
	if (par2 != NULL) {
		par_num++;
	}
	if (par3 != NULL) {
		par_num++;
	}
	// Copy function name
	int i = 0;
	while (name[i] != '\0') {
		new_line[i] = name[i];
		i++;
	}
	new_line[i++] = ' ';
	// Copy first parameter
	if (par1 == NULL) {
		printf("Error: Parameter 1 is null.\n");
		return false;
	}
	int j = 0;
	while (par1[j] != '\0') {
		new_line[i] = par1[j];
		i++;
		j++;
	}
	if (par_num > 1) {
		new_line[i++] = ',';
		new_line[i++] = ' ';
	}
	// Copy second parameter
	if (par2 == NULL) {
		printf("Error: Parameter 2 is null.\n");
		return false;
	}
	j = 0;
	while (par2[j] != '\0') {
		new_line[i] = par2[j];
		i++;
		j++;
	}
	if (par_num > 2) {
		new_line[i++] = ',';
		new_line[i++] = ' ';
	}
	// Copy third parameter.
	if (par3 != NULL) {
		j = 0;
		while (par3[j] != '\0') {
			new_line[i] = par3[j];
			i++;
			j++;
		}
	}
	new_line[i++] = '\n';
	new_line[i] = '\0';

	return true;
}
