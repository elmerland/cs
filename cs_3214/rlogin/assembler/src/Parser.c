/*
 * Parser.c
 * 	This source file handles the interpretation of each line of code in the in
 * 	the input file. After the preprocessor is done cleaning the file, it is the
 * 	job of the parser to extract all the relevant information from each line of
 * 	code in the assembly file. It then puts all this information in the
 * 	appropriate table.
 *
 *  Created on: Oct 20, 2013
 *      Author: Elmer Landaverde
 */
#include "Global.h"
#include "Utilities.h"
#include "StructUtilities.h"
#include "Parser.h"

/**
 *	get_sym_tbl() this functions takes a symbol table and populates it with all
 *				the labels in the input file. The symbol table should have a
 *				length as specified by get_cnt(file, SYMBOL).
 *	Pre:		Input points to a readable file.
 *				Sym_tbl points to a properly initialized array of Symbol_Info
 *				objects of size get_cnt(file, SYMBOL);
 *	Post:		The input file has not being modified.
 *				The sym_tbl now contains the names of all the labels in the
 *				input file.
 *	Returns:	True if the all the labels could be obtained successfully.
 *				Otherwise returns false.
 */
static bool gen_sym_tbl(FILE *input);

/**
 *	gen_inst_tbl() This function goes through every line of the input file and
 *				whenever it finds an instruction line it processes the
 *				information and puts it in the appropriate Instruction Info
 *				structure. It also updates the addresses of any labels located
 *				in the text section of the file.
 *	Pre:		Input points to a readable file.
 *				Inst_tbl points to a properly initialized array of Information
 *				Info structure of length get_cnt(INSTRUCTION).
 *				Sym_tbl is the table of symbols to be updated.
 *				Sym_cnt is the size of the Sym_tbl array.
 *	Post:		All the Instruction Info structures have being updated.
 *				All the symbols in the text section have being updated.
 *	Return:		True if all the information was processed successfully.
 *				Otherwise it returns false.
 */
static bool gen_inst_tbl(FILE *input);

/**
 *	gen_var_tbl() This function takes in a clean asm file and extracts all the
 *				information it needs to fill out the variable info table.
 *	Pre:		Input points to a reable file.
 *				Var_tbl points to a properly initialized table.
 *				Sym_tbl points to a properly initialized symbols table
 *				containing all the symbols in the table.
 *				Sym_cnt contains the size of the symbols table.
 *	Post:		The variables table has being filled out completely.
 *				The missing addresses in the symbols table have being completed.
 *	Returns:	True if the variables table was filled completely and the
 *				symbols table was also completed. Returns false otherwise.
 */
static bool gen_var_tbl(FILE *input);

/**
 *	fill_inst_inf() This function takes a line of assembly code and an
 *				Instruction Info structure, and the proceeds to extract all the
 *				necessary information from the line of assembly code to
 *				assign values to all the variables of the Instruction Info
 *				structure.
 *	Pre:		Inst points to a properly initialized Instruction Info
 *				structure.
 *				Addr contains the address of the current line.
 *				Line contains the line of assembly code to be used.
 *				Label is the label at this address if any (Set to NULL if no
 *				label exists).
 *	Post:		The variables of inst have being update to reflect the contents
 *				of the line of assembly code.
 *	Return:		True if the Instruction Info structure was successfully filled.
 *				Otherwise it returns false.
 */
static bool fill_inst_inf(Instruction_Info * const inst, uint32_t addr,
		const char * const line);

/**
 *	fill_var_inf() This function takes in a Variable_Info structure and line of
 *				code from the input file and proceed to extract all the relevant
 *				information from the line of code and copy it over to the
 *				variable structure.
 *	Pre:		Var points to a properly initialized structure.
 *				Addr contains the current address.
 *				Line contains the current line.
 *				Label contains the current label.
 *	Post:		Var has being filled with all the relevant information.
 *	Returns:	The next address to be used. Returns 0 if the variable structure
 *				could not be filled.
 */
static bool fill_var_inf(Variable_Info * const var, const char * const line,
		const char * const label);

/**
 *	update_sym_tbl() This function updates the address of the specified symbol.
 *	Pre:		Symbol is the name of the symbol to be updated.
 *				Addr is the address of the symbol to be updated.
 *				Sym_tbl is a properly initialized symbols table.
 *				Sym_cnt is the size of the symbols table.
 *	Post:		The address of the symbol matching the given symbol name, has
 *				being updated.
 *	Return:		True if the address was updated successfully. Otherwise returns
 *				false.
 */
static bool update_sym_tbl(const char * const symbol, uint32_t addr);

/**
 *	set_var_addr() This function determines the starting address of the current
 *				variable info structure. The current variable address depends
 *				on the following:
 *				- 	If the previous variable is a string, and the current
 *					variable is also a string; or if the previous variable is
 *					not a string then the starting address of the
 *					current variable is equal to the starting address of the
 *					previous variable plus the length in bytes of the previous
 *					variable.
 *				-	If the previous variable is a string, and the current
 *					variable is not a string, then the starting address of the
 *					current variable is the equals to the starting address of
 *					the previous variable plus the length in bytes of the
 *					previous variable plus an offset so that the address of the
 *					current variable is a multiple of 4.
 *	Pre:		The current index represents the index of the variable info
 *				structure that was just filled.
 *	Post:		The variable at the current index has a correct starting
 *				address.
 *	Return:		True if the current address was computed successfully, false
 *				otherwise.
 */
static bool set_var_addr(uint32_t curr_indx);

/**
 *	get_inst_type() This function takes in an instruction name and determines
 *				what type of instruction it is.
 *	Pre:		Name points to the name of an instruction.
 *	Post:		Name remains unchanged.
 *	Returns:	The Instruction_Type that corresponds to the type of instruction
 *				that was passed in. If the name does not match any instruction
 *				the I_NOT_DEFF will be returned.
 */
static Instruction_Type get_inst_type(const char * const name);

/**
 *	get_var_type() Determines what type of variable the give variable name
 *				belongs to.
 *	Pre:		Name is the name of the variable.
 *				line contains the current line of code.
 *	Post:		All parameters remained unchanged.
 *	Returns:	The type of variable that name belongs to. Returns V_NOT_DEF if
 *				name does not match any known variable types.
 */
static Variable_Type get_var_type(const char * const name,
		const char * const line);

/**
 *	get_cnt() This function counts all the instances of the specified type that
 *				appear in the input file. This function is capable of counting
 *				instructions, variables, and labels.
 *	Pre:		Input points to readable file.
 *				Type indicates what type of line the function is going to count.
 *	Post:		The input file remains unchanged.
 *				The type remains unchanged.
 *	Returns:	The number of occurrences of the specified type in the input
 *				file.
 */
static uint32_t get_cnt(FILE *input, Info_Type type);

/*
 * All known instruction types.
 */
const char * const R_TYPES[] = { "add", "sub", "addi", "addiu", "or", "and",
		"ori", "andi", "slt", "slti", "sll", "srl", "sra" };
const char * const I_TYPES[] = { "lw", "sw" };
const char * const J_TYPES[] = { "beq", "bne", "bltz", "blez", "j", "jr", "jal",
		"syscall" };

/*
 * The length of each of the instruction types arrays.
 */
const uint32_t R_CNT = 13;
const uint32_t I_CNT = 2;
const uint32_t J_CNT = 8;

/*
 * The names of the known variable types.
 */
const char * const WORD_TYPE = ".word";
const char * const ASCII_TYPE = ".asciiz";

/*
 * Tables to instructions, symbols, and variables.
 */
Symbol_Info *sym_tbl = NULL;
Instruction_Info *inst_tbl = NULL;
Variable_Info *var_tbl = NULL;

/*
 * The size of the tables.
 */
uint32_t sym_cnt = 0;
uint32_t inst_cnt = 0;
uint32_t var_cnt = 0;

bool parse(FILE *input) {

	// Get dimension of arrays.
	sym_cnt = get_cnt(input, SYMBOL);
	inst_cnt = get_cnt(input, INSTRUCTION);
	var_cnt = get_cnt(input, VARIABLE);

	// Allocate memory for arrays.
	sym_tbl = (Symbol_Info*) calloc(sym_cnt, sizeof(Symbol_Info));
	if (sym_tbl == NULL) {
		printf("Error: Could not allocate memory for symbol table.\n");
		return false;
	}
	inst_tbl = (Instruction_Info*) calloc(inst_cnt, sizeof(Instruction_Info));
	if (inst_tbl == NULL) {
		printf("Error: Could not allocate memory for instruction table.\n");
		return false;
	}
	var_tbl = (Variable_Info*) calloc(var_cnt, sizeof(Variable_Info));
	if (var_tbl == NULL) {
		printf("Error: Could not allocate memory for variable table.\n");
		return false;
	}

	// Initialize arrays.
	set_sym_inf_a(sym_tbl, sym_cnt);
	set_inst_inf_a(inst_tbl, inst_cnt);
	set_var_inf_a(var_tbl, var_cnt);

	// Generate symbol table.
	bool success = gen_sym_tbl(input);
	if (!success) {
		printf("Error: Could not generate symbol table.\n");
		return false;
	}

	// Generate instruction table.
	success = gen_inst_tbl(input);
	if (!success) {
		printf("Error: Could not generate instruction table.\n");
		return false;
	}

	// Generate variables table.
	success = gen_var_tbl(input);
	if (!success) {
		printf("Error: Could not generate variable table.\n");
		return false;
	}
	return true;
}

static bool gen_inst_tbl(FILE *input) {
	char ln[MAX_LN_LEN];
	uint32_t addr = TEXT_START_ADDR;
	int i = 0;
	bool has_started = false;
	bool has_label = false;
	char *section_start = ".text\n";
	char *section_end = ".data\n";
	char curr_lbl[LABEL_LEN];
	while (fgets(ln, MAX_LN_LEN, input) != NULL) {
		if (!has_started && strncmp(section_start, ln, 6) == 0) {
			has_started = true;
			continue;
		} else if (has_started && strncmp(section_end, ln, 6) == 0) {
			// Stop reading instructions.
			break;
		}

		if (has_lbl(ln)) {
			// Copy label to temporary string.
			strncpy(curr_lbl, ln, LABEL_LEN);
			clean_lbl(curr_lbl);
			has_label = true;
			continue;
		}

		if (has_started) {
			// Process this instruction line.
			Instruction_Info *inst = &inst_tbl[i];
			if (inst == NULL) {
				printf("Error: Could not get Instruction Info structure.\n");
				return false;
			}
			// Fill variables of the instruction info.
			bool success = fill_inst_inf(inst, addr, ln);
			if (!success) {
				printf("Error: Could not fill instruction info.\n");
				return false;
			}
			if (has_label) {
				// Update symbol table.
				bool result = update_sym_tbl(curr_lbl, addr);
				if (!result) {
					printf("Error: Could not update the symbol table.\n");
					return false;
				}
				has_label = false;
			}
			addr += 4;
			i++;
		}
	}
	rewind(input);
	return true;
}

static bool fill_inst_inf(Instruction_Info * const inst, uint32_t addr,
		const char * const line) {

	// Copy address.
	inst->addr = addr;

	// Copy original instruction.
	inst->line = malloc(sizeof(char) * MAX_LN_LEN);
	if (inst->line == NULL) {
		printf("Error: Could not allocate memory for instruction.\n");
		return false;
	}
	strncpy(inst->line, line, MAX_LN_LEN);

	// Get copy of line to tokenize.
	char line_cpy[MAX_LN_LEN];
	strncpy(line_cpy, line, MAX_LN_LEN);
	// Remove new line character from line.
	int i = 0;
	while (line_cpy[i] != '\0') {
		if (line_cpy[i] == '\n') {
			line_cpy[i] = '\0';
			break;
		}
		i++;
	}

	// Get instruction name.
	char *name = strtok(line_cpy, INST_DELIMETER);
	if (name == NULL || strlen(name) == 0) {
		printf("Error: Could not get instruction name.\n");
		return false;
	}

	// Get parameters
	char *par1 = strtok(NULL, INST_DELIMETER);
	char *par2 = strtok(NULL, INST_DELIMETER);
	char *par3 = strtok(NULL, INST_DELIMETER);

	// Copy instruction name
	inst->inst = malloc(sizeof(char) * TKN_LEN);
	if (inst->inst == NULL) {
		printf("Error: Could not allocate memory for instruction name.\n");
		return false;
	}
	strncpy(inst->inst, name, TKN_LEN);
	// Copy parameter 1
	if (par1 != NULL) {
		inst->par1 = malloc(sizeof(char) * TKN_LEN);
		if (inst->par1 == NULL) {
			printf("Error: Could not allocate memory for parameter 1.\n");
			return false;
		}
		strncpy(inst->par1, par1, TKN_LEN);
	}
	// Copy parameter 2
	if (par2 != NULL) {
		inst->par2 = malloc(sizeof(char) * TKN_LEN);
		if (inst->par2 == NULL) {
			printf("Error: Could not allocate memory for parameter .2\n");
			return false;
		}
		strncpy(inst->par2, par2, TKN_LEN);
	}
	// Copy parameter 3
	if (par3 != NULL) {
		inst->par3 = malloc(sizeof(char) * TKN_LEN);
		if (inst->par3 == NULL) {
			printf("Error: Could not allocate memory for parameter 3.\n");
			return false;
		}
		strncpy(inst->par3, par3, TKN_LEN);
	}

	// Get instruction type.
	inst->type = get_inst_type(name);
	if (inst->type == I_NOT_DEF) {
		printf("Error: Could not determine instruction type.\n");
		return false;
	}

	// Instruction successfully filled.
	return true;
}

static Instruction_Type get_inst_type(const char * const name) {
	if (name == NULL) {
		printf("Error: Name pointer is NULL.\n");
		return I_NOT_DEF;
	}

	// Check if instruction is R type.
	for (uint32_t i = 0; i < R_CNT; i++) {
		if (strncmp(name, R_TYPES[i], TKN_LEN) == 0) {
			return R_TYPE;
		}
	}
	// Check if instruction is R type.
	for (uint32_t i = 0; i < I_CNT; i++) {
		if (strncmp(name, I_TYPES[i], TKN_LEN) == 0) {
			return I_TYPE;
		}
	}
	// Check if instruction is R type.
	for (uint32_t i = 0; i < J_CNT; i++) {
		if (strncmp(name, J_TYPES[i], TKN_LEN) == 0) {
			return J_TYPE;
		}
	}
	printf("Error: Instruction did not match any known instructions:\t%s\n",
			name);
	return I_NOT_DEF;
}

static bool update_sym_tbl(const char * const symbol, uint32_t addr) {
	for (uint32_t i = 0; i < sym_cnt; i++) {
		if (strncmp(symbol, sym_tbl[i].symbol, LABEL_LEN) == 0) {
			sym_tbl[i].addr = addr;
			return true;
		}
	}
	return false;
}

static bool gen_var_tbl(FILE *input) {
	char ln[MAX_LN_LEN];
	uint32_t i = 0;
	bool has_started = false;
	bool has_label = false;
	char *section_start = ".data\n";
	char *section_end = ".text\n";
	char curr_lbl[LABEL_LEN];
	while (fgets(ln, MAX_LN_LEN, input) != NULL) {
		if (!has_started && strncmp(section_start, ln, 6) == 0) {
			has_started = true;
			continue;
		} else if (has_started && strncmp(section_end, ln, 6) == 0) {
			// Stop reading instructions.
			break;
		}

		if (has_lbl(ln)) {
			// Copy label to temporary string.
			strncpy(curr_lbl, ln, LABEL_LEN);
			clean_lbl(curr_lbl);
			has_label = true;
			continue;
		}

		if (has_started) {
			Variable_Info *var = &var_tbl[i];
			if (var == NULL) {
				printf("Error: Could not obtain variable info.\n");
				return false;
			}
			// Fill variable information.
			bool result = fill_var_inf(var, ln, curr_lbl);
			if (!result) {
				printf("Error: Could not fill variable information.\n");
				return false;
			}
			// Get starting address.
			result = set_var_addr(i);
			if (!result) {
				printf("Error: Could not get starting address.\n");
				return false;
			}
			// Update label information.
			if (!has_label) {
				printf("Error: Could find label for variable.\n");
				return false;
			}
			result = update_sym_tbl(curr_lbl, var_tbl[i].addr);
			if (!result) {
				printf("Error: Could not update the symbol table.\n");
				return false;
			}
			has_label = false;
			i++;
		}
	}
	rewind(input);
	return true;
}

static bool fill_var_inf(Variable_Info * const var, const char * const line,
		const char * const label) {
	// Copy original instruction.
	var->line = malloc(sizeof(char) * MAX_LN_LEN);
	if (var->line == NULL) {
		printf("Error: Could not allocate memory for variable.\n");
		return false;
	}
	strncpy(var->line, line, MAX_LN_LEN);

	// Copy label.
	var->label = malloc(sizeof(char) * LABEL_LEN);
	if (var->label == NULL) {
		printf("Error: Could not allocate memory for variable.\n");
		return false;
	}
	strncpy(var->label, label, LABEL_LEN);

	// Get copy of line to tokenize.
	char line_cpy[MAX_LN_LEN];
	strncpy(line_cpy, line, MAX_LN_LEN);
	// Remove new line character from line.
	int i = 0;
	while (line_cpy[i] != '\0') {
		if (line_cpy[i] == '\n') {
			line_cpy[i] = '\0';
			break;
		}
		i++;
	}

	// Get variable name.
	char *name = strtok(line_cpy, VAR_DELIMETER);
	if (name == NULL || strlen(name) == 0) {
		printf("Error: Could not get variable type.\n");
		return false;
	}
	var->name = malloc(sizeof(char) * TKN_LEN);
	if (var->name == NULL) {
		printf("Error: Could not allocate memory for name.\n");
		return false;
	}
	strncpy(var->name, name, TKN_LEN);

	// Get variable type.
	var->type = get_var_type(name, line);

	// Get parameters.
	if (var->type == V_NOT_DEF) {
		printf("Error: The variable type is not defined.\n");
		return false;
	} else if (var->type == WORD || var->type == WORD_A
			|| var->type == WORD_A_S) {
		var->parameter = malloc(sizeof(char) * TKN_LEN);
		if (var->parameter == NULL) {
			printf("Error: Could not allocate memory for parameter.\n");
			return false;
		}
		// Get parameter token.
		char *par;
		if (var->type == WORD || var->type == WORD_A_S) {
			par = strtok(NULL, "\0");
		} else {
			par = strtok(NULL, VAR_A_DELIMETER);
		}
		// Check the resulting token.
		if (par == NULL) {
			printf("Error: Could not get parameter token.\n");
			return false;
		}
		strncpy(var->parameter, par, TKN_LEN);

		// Get length
		if (var->type == WORD_A_S || var->type == WORD) {
			// Get array length for set integer array.
			// Create copy of parameter.
			char par_temp[TKN_LEN];
			strncpy(par_temp, var->parameter, TKN_LEN);
			// Get numbers
			uint32_t num_count = 0;
			char *num_temp = strtok(par_temp, VAR_A_S_DELIMETER);
			while (num_temp != NULL && strlen(num_temp) > 0) {
				num_count++;
				num_temp = strtok(NULL, VAR_A_S_DELIMETER);
			}
			var->length = num_count;
		} else {
			// Get array length for integer array.
			char *length = strtok(NULL, VAR_A_DELIMETER);
			if (length == NULL) {
				printf("Error: Could not get length of integer array.");
				return false;
			}
			var->length = parse_int(length);
		}
	} else {
		// Get parameter for ASCII type.
		var->parameter = malloc(sizeof(char) * TKN_LEN);
		if (var->parameter == NULL) {
			printf("Error: Could not allocate memory for parameter.\n");
			return false;
		}
		// Get parameter token.
		uint32_t j = 0;
		bool has_started = false;
		for (int i = 0; line[i] != '\0'; i++) {
			// Check if quotation starts.
			if (!has_started && line[i] == '\"') {
				has_started = true;
				continue;
			}
			// Check if quotation ends.
			if (has_started && line[i] == '"') {
				var->parameter[j] = '\0';
				break;
			}
			// Copy relevant character to parameter field.
			if (has_started) {
				var->parameter[j] = line[i];
				j++;
			}
		}
		var->length = j + 1;
	}

	// Get the byte length of the variable.
	if (var->type == ASCII) {
		var->byte_length = var->length;
	} else if (var->type == WORD || var->type == WORD_A
			|| var->type == WORD_A_S) {
		var->byte_length = var->length * 4;
	}
	return true;
}

static Variable_Type get_var_type(const char * const name,
		const char * const line) {
	if (strncmp(name, WORD_TYPE, TKN_LEN) == 0) {
		// Check if type is WORD or WORD_A
		int i = 0;
		while (line[i] != '\0') {
			if (line[i] == ':') {
				return WORD_A;
			} else if (line[i] == ',') {
				return WORD_A_S;
			}
			i++;
		}
		return WORD;
	}
	if (strncmp(name, ASCII_TYPE, TKN_LEN) == 0) {
		return ASCII;
	}
	return V_NOT_DEF;
}

static bool set_var_addr(uint32_t curr_indx) {
	// Check input.
	if (curr_indx >= var_cnt) {
		printf("Error: Current index is invalid.\n");
		return 0;
	}
	// Address for first variable.
	if (curr_indx == 0) {
		var_tbl[curr_indx].addr = DATA_START_ADDR;
		return true;
	}
	bool is_prev_str = (var_tbl[curr_indx - 1].type == ASCII);
	bool is_curr_str = (var_tbl[curr_indx].type == ASCII);
	if (!is_prev_str || (is_prev_str && is_curr_str)) {
		// Get previous starting address.
		uint32_t prev_addr = var_tbl[curr_indx - 1].addr;
		// Get previous byte length.
		uint32_t prev_b_len = var_tbl[curr_indx - 1].byte_length;
		// Calculate current starting address.
		uint32_t curr_addr = prev_addr + prev_b_len;
		var_tbl[curr_indx].addr = curr_addr;
		return true;
	} else if (is_prev_str && !is_curr_str) {
		// Get previous starting address.
		uint32_t prev_addr = var_tbl[curr_indx - 1].addr;
		// Get previous byte length.
		uint32_t prev_b_len = var_tbl[curr_indx - 1].byte_length;
		// Calculate current starting address.
		uint32_t curr_addr = prev_addr + prev_b_len;
		uint32_t mod = curr_addr % 4;
		if (mod != 0) {
			curr_addr += 4 - mod;
		}
		var_tbl[curr_indx].addr = curr_addr;
		return true;
	}
	printf("Error: Unknown error.");
	return false;
}

static bool gen_sym_tbl(FILE *input) {
	char ln[MAX_LN_LEN];
	int i = 0;
	while (fgets(ln, MAX_LN_LEN, input) != NULL) {
		if (has_lbl(ln)) {
			// Clean label
			clean_lbl(ln);
			// Get Symbol Info structure.
			Symbol_Info *sym = &sym_tbl[i];
			if (sym == NULL) {
				printf("Error: Could not get Symbol Info structure.\n");
				return false;
			}
			// Copy label.
			sym->symbol = malloc(sizeof(char) * LABEL_LEN);
			if (sym->symbol == NULL) {
				printf("Error: Could not allocate memory for symbol.\n");
				return false;
			}
			strncpy(sym->symbol, ln, LABEL_LEN);
			i++;
		}
	}
	rewind(input);
	return true;
}

static uint32_t get_cnt(FILE *input, Info_Type type) {
	if (input == NULL) {
		printf("Error: File pointer is null.\n");
		return 0;
	}
	uint32_t cnt = 0;
	char ln[MAX_LN_LEN];
	while (fgets(ln, MAX_LN_LEN, input) != NULL) {
		bool is_lbl = has_lbl(ln);
		bool is_sec = ln[0] == '.';
		bool is_var = (ln[1] == 'a' || ln[1] == 'w');
		if (type == INSTRUCTION) {
			if (!is_lbl && !is_sec) {
				cnt++;
			}
		} else if (type == SYMBOL) {
			if (is_lbl) {
				cnt++;
			}
		} else if (type == VARIABLE) {
			if (is_sec && is_var) {
				cnt++;
			}
		}
	}
	rewind(input);
	return cnt;
}
