/*
 * Assembler.c
 * 	Assembler project for CS 2506. This project was created by Elmer Landaverde
 * 	ID 9054-91691. Please refer to the READ ME file for more information or use
 * 	the --help option to get a quick overview of the program.
 *
 *  Created on: Oct 20, 2013
 *      Author: Elmer Landaverde
 */

/*
 *	this is sooooo cool!!!!
 */

#include "Assembler.h"

/**
 *	Preprocess() This function cleans the input file. It removes all leading
 *				spaces or tabs. Eliminates empty lines. Eliminates comments
 *				either leading or trailing. It replaces all sudo instruction
 *				with actual instructions.
 *	Pre:		The input file pointer points to a valid readable file.
 *				The clean file pointer points to a valid writable file.
 *	Post:		The input file does not change.
 *				The clean file contains the cleaned input.
 */
static bool preprocess_asm();

/**
 *	parse_asm() This functions takes the cleaned input from the preprocessor and
 *				proceeds to extract information from each line. When the parser
 *				is done all the instructions table, symbols table and variables
 *				table are completed and filled with all the relevant info.
 *	Pre:		The input file has being cleaned.
 *	Post:		All tables have being filled.
 *	Return:		True if the parsing was successful, otherwise returns false.
 */
static bool parse_asm();

/**
 *	translate_asm() This function takes the information generated by the parser
 *				and produces the binary representation of the information.
 *	Pre:		The instructions, symbols and variables tables are complete.
 *	Post:		The binary line in the Instruction Info structure has being
 *				filled out completely.
 *	Return:		True if the translation was successful, otherwise false.
 */
static bool translate_asm();

/**
 *	Create_Temporary_Files() This function creates all the temporary files that
 *				will be needed for the assembler.
 *	Pre:		Input file is a null pointer.
 *				Output file is a null pointer.
 *				Clean file is a null pointer.
 *				Text file is a null pointer.
 *				Data file is a null pointer.
 *	Post:		Input file points to a readable file.
 *				Output file points to writable file.
 *				Clean file points to a readable and writable file.
 *				Text file points to a readable and writable file.
 *				Data file points to a readable and writable file.
 */
static bool create_temp_files();

/**
 *	proces_args() Takes the length of the arguments and the arguments given to
 *				the main function. It first validates the arguments, and then
 *				sets all the appropriate variables to reflect the contents of
 *				the arguments. Sets the symbols_flag, list_flag, input, and
 *				output variables to	reflect the contents of the input arguments.
 *	Pre:		Argslen contains the length of the args array.
 *				Args is a properly initialize array of strings.
 *	Post:		Argslen has not being changed.
 *				Args has not being changed.
 *	Returns:	True if the arguments contain valid information and the
 *				relevant variables could be set successfully. Returns false
 *				otherwise. If this function returns false an appropriate message
 *				describing what went wrong will be printed.
 */
static bool process_args(int argc, char **argv);

/**
 *	validate_args() Verifies that the input given to the main function is valid.
 *				For the input to be valid the number of arguments cannot
 *				exceed MAX_ARGS and cannot be below MIN_ARGS. The input can have
 *				a flag only if it has two other	arguments (Meaning 4 arguments
 *				in total).
 *	Pre:		Argslen contains the length of the args array.
 *				Args is a properly initialize array of strings.
 *	Post:		Argslen has not being changed.
 *				Args has not being changed.
 *	Returns:	True if the arguments are valid according to the rules stated
 *				above. Returns false otherwise. If this function returns false
 *				an appropriate messages describing what went wrong will be
 *				printed to console.
 */
static bool validate_args(int argc, char **argv);

/**
 *	Close_Files() This function makes sure that all the file pointers are closed
 *				before the main function exits. The file pointer will be closed
 *				only if they are not null (in case they could not be properly
 *				initialized).
 *	Pre:		The file pointer variables have being declared.
 *	Post:		If any of the files pointers are not they will be closed using
 *				fclose(), if the fclose function does not return successfully
 *				then an appropriate message will be printed to console.
 */
static void close_files();

/*
 *	print_helper_message() Prints a message describing the usage of this
 *				program.
 */
static void print_helper_message();

// File pointers
FILE *input = NULL;
FILE *output = NULL;

FILE *clean_file = NULL;

// Boolean variables for the arguments flags.
bool has_options = false;
bool symbols_option = false;
bool list_option = false;

// Various strings needed for comparison.
char *symbols_string_flag = "-symbols";
char *list_string_flag = "-list";

char *clean_file_name = "clean.temp";

int main(int argc, char **argv) {
	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
		print_helper_message();
		return 0;
	}
	// Validate and process the input arguments.
	printf("> Analyzing arguments ... ");
	bool success = process_args(argc, argv);
	if (!success) {

		close_files();
		return 1;
	}
	printf("Done\n");
	
	// Create temporary files.
	printf("> Creating temporary files ... ");
	success = create_temp_files();
	if (!success) {
		close_files();
		return 1;
	}
	printf("Done\n");
	
	// Preprocess the input.
	printf("> Preprocessing ... ");
	success = preprocess_asm();
	if (success == false) {
		close_files();
		free_parser_arrays();
		return 1;
	}
	printf("Done\n");
	
	// Parse the input.
	printf("> Parsing ... ");
	success = parse_asm();
	if (!success) {
		close_files();
		free_parser_arrays();
		return 1;
	}
	printf("Done\n");

	// Translate the input
	printf("> Translating ... ");
	success = translate_asm();
	if (!success) {
		close_files();
		free_parser_arrays();
		return 1;
	}
	printf("Done\n");
	printf("> Printing results to file ... ");

	// Print output to file
	print_output(list_option, symbols_option, output);
	
	printf("Done\n");
	close_files();
	free_parser_arrays();
	printf("DFTBA!\n");
	return 0;
}

static bool create_temp_files() {
	// Create temporary file to store clean input.
	clean_file = fopen(clean_file_name, "w+");
	if (clean_file == NULL) {
		printf("Error: Could not create clean up text file.\n");
		return false;
	}
	return true;
}

static bool preprocess_asm() {
	// Clean input file.
	uint32_t line_count = preprocess(input, clean_file);
	if (line_count <= 0) {
		printf("Error: Could not preprocess the file.\n");
		return false;
	}
	//prnt_txt_file(clean_file);

	return true;
}

static bool parse_asm() {
	bool success = parse(clean_file);
	if (!success) {
		printf("Error: Could not parse the file.\n");
		return false;
	}

	return true;
}

static bool translate_asm() {
	bool success = translate();
	if (!success) {
		printf("Error: Could not translate the file.\n");
		return false;
	}

	return true;
}

static void close_files() {
	printf("> Closing files ... ");
	if (input != NULL) {
		if (fclose(input) != 0) {
			printf("Error: Could not close the input file.\n");
		}
	}
	else {
		printf("Error: Input file is NULL\n");
	}
	
	if (output != NULL) {
		if (fclose(output) != 0) {
			printf("Error: Could not close the output file.\n");
		}
	}
	else {
		printf("Error: Ouput file is NULL\n");
	}

	if (clean_file != NULL) {
		if (fclose(clean_file) != 0) {
			printf(
					"Error: Intermediate clean input file could not be closed.\n");
		}
		if (remove(clean_file_name) != 0) {
			printf(
					"Error: Intermediate clean input file could not be deleted.\n");
		}
	}
	else {
		printf("Error: Clean file is NULL\n");
	}
	printf("Done\n");
}

static bool process_args(int argc, char **argv) {
	// Validate arguments.
	if (!validate_args(argc, argv)) {
		return false;
	}

	// Get testing flag

	// Determine if the input has flags.
	if (argv[1][0] == '-') {
		has_options = true;
	}

	// Get the index for each argument.
	int options_index, input_index, output_index;
	if (has_options) {
		options_index = 1;
		input_index = 2;
		output_index = 3;
	} else {
		options_index = -1;
		input_index = 1;
		output_index = 2;
	}

	// Get flag type.
	if (has_options) {
		char *flag = argv[options_index];
		if (strcmp(flag, list_string_flag) == 0) {
			list_option = true;
		} else if (strcmp(flag, symbols_string_flag) == 0) {
			symbols_option = true;
		} else {
			printf("Error: Arguments contain unknown flag type.\n");
			return false;
		}
	}

	// Get input file
	input = fopen(argv[input_index], "r");
	if (input == NULL) {
		printf("Error: The input file pointer could not be obtained.\n");
		return false;
	}

	// Get output file
	output = fopen(argv[output_index], "w+");
	if (output == NULL) {
		printf("Error: The output file pointer could not be obtained.\n");
		return false;
	}

	return true;
}

static bool validate_args(int argc, char **argv) {
	// Validate number of arguments
	if (argc < MIN_ARGS) {
		printf("Error: Number of arguments is too low.\n");
		return false;
	} else if (argc > MAX_ARGS) {
		printf("Error: Number of arguments is too high.\n");
		return false;
	}

	// Check for flags. A flag should exist only if there are four arguments or more.
	char *flag = argv[1];
	if (flag[0] != '-' && argc >= MAX_ARGS) {
		// There are four arguments and no flag.
		printf("Error: First argument should be a flag.\n");
		return false;
	} else if (flag[0] == '-' && argc == MIN_ARGS) {
		// There is a flag but too few arguments.
		printf("Error: Missing input or output file.\n");
		return false;
	}
	return true;
}

static void print_helper_message() {
	printf("USAGE:\n");
	printf("\t\tassemble [-list|-symbols] [SOURCE] [DEST]\n\n");
	printf("DESCRIPTION:\n");
	printf(
			"-list\t\tOutput a list representation to the destination file.\n\n");
	printf(
			"-symbols\tOutput a symbol representation to the destination file.\n\n");
	printf("SOURCE\t\tThe file name of the input file that contains the MIPS\n"
			"\t\tinstructions to be assembled.\n\n");
	printf("DEST\t\tThe file name of the output file where the results of\n"
			"\t\tthe assembler will be written to.\n"
			"\t\tNOTE: File does not have to exist.\n");
}
