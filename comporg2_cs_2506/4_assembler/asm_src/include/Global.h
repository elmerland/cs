/*
 * Global.h
 * 	Contains all the global structures, enumerators, and tables. This header
 * 	file helps to better organize the structure of the program. Most other
 * 	headers need to include only this file.
 *
 *  Created on: Nov 14, 2013
 *      Author: Elmer Landaverde
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <inttypes.h>

////////////////////////////////////////////////////////////////////////////////
// Global defined constants.
////////////////////////////////////////////////////////////////////////////////

#define MAX_LN_LEN 257 			// Maximum length of a line of assembler code.
#define WORD_LEN 33				// Maximum length of a binary line.
#define CHAR_LEN 8				// Maximum length for a binary representation of a character.
#define BIN_LN_LEN 33 			// Length of string to represent 32-bit word.
#define LABEL_LEN 33			// Maximum length of string to hold label.
#define TKN_LEN 257				// Maximum length of string to hold a parameter.
#define TEXT_START_ADDR 0		// Starting address for text section.
#define DATA_START_ADDR 8192	// Starting address for data section.
#define TOKEN_SEPARATOR ' ' 	// Delimiter for word tokens in assembler file.
#define DELIMETER " ," 			// Delimiter for comma separated values in assembler file.
#define INST_DELIMETER " ,()"	// Delimiter for instruction tokens.
#define VAR_DELIMETER " "		// Delimiter for variable tokens.
#define VAR_A_DELIMETER " :"	// Delimiter for array variables of set values.
#define VAR_A_S_DELIMETER " ,"	// Delimiter for array variables.
#define VAR_S_DEILIMETER "\""	// Delimiter for string variables.

////////////////////////////////////////////////////////////////////////////////
// Global string constants.
////////////////////////////////////////////////////////////////////////////////

extern const char * const WORD_TYPE; // Constant for WORD variables.
extern const char * const ASCII_TYPE; // Constant for ASCIIZ variables.

////////////////////////////////////////////////////////////////////////////////
// Enumerators
////////////////////////////////////////////////////////////////////////////////

/**
 * Enumerator that lists all the types of lines in an assembler file.
 */
enum _Info_Type {
	INSTRUCTION, VARIABLE, SYMBOL
};
/**
 * Types of instructions lines in the text section.
 */
enum _Instruction_Type {
	R_TYPE, I_TYPE, J_TYPE, I_NOT_DEF
};
/**
 * Types of variables in the data section.
 */
enum _Variable_Type {
	WORD, WORD_A, WORD_A_S, ASCII, V_NOT_DEF
};

////////////////////////////////////////////////////////////////////////////////
// Global enumerator type definitions.
////////////////////////////////////////////////////////////////////////////////

typedef enum _Info_Type Info_Type;
typedef enum _Instruction_Type Instruction_Type;
typedef enum _Variable_Type Variable_Type;

////////////////////////////////////////////////////////////////////////////////
// Structures
////////////////////////////////////////////////////////////////////////////////

/*
 * Structure to hold all the information for an instruction.
 */
struct _Instruction_Info {
	// Holds the address of the instruction.
	uint32_t addr;
	// Holds any label that is located at this instruction if any.
	//char* label;
	// The original line of assembler code.
	char* line;
	// The mnemonic of the instruction.
	char* inst;
	// The first parameter of the instruction if any.
	char* par1;
	// The second parameter of the instruction if any.
	char* par2;
	// The third parameter of the instruction if any.
	char* par3;
	// The type of instruction.
	Instruction_Type type;
	// The binary representation of this instruction.
	char *binary;
};
/*
 * Structure to hold all the information for a symbol.
 */
struct _Symbol_Info {
	char *symbol;
	uint32_t addr;
};
/*
 * Structure to hold all the information for a variable.
 */
struct _Variable_Info {
	// Starting address of the variable.
	uint32_t addr;
	// The label of this variable.
	char* label;
	// The original line of assembly code.
	char *line;
	// The name of the type of variable.
	char *name;
	// The parameter of the variable.
	char *parameter;
	// The length in bytes of this variable.
	uint32_t length;
	// The length in byte words of this variable.
	uint32_t byte_length;
	// The type of variable.
	Variable_Type type;
};
/*
 * Structure to hold a key and record for a table.
 */
struct _Dictionary {
	char *name;
	char *bin;
};
/*
 * Structure to hold all the information to identify the position of a parameter
 * in the binary line.
 */
struct _Parameter_Info {
	uint32_t star_pos;
	uint32_t size;
};

////////////////////////////////////////////////////////////////////////////////
// Global structure type definitions.
////////////////////////////////////////////////////////////////////////////////

typedef struct _Instruction_Info Instruction_Info;
typedef struct _Symbol_Info Symbol_Info;
typedef struct _Variable_Info Variable_Info;
typedef struct _Dictionary Dictionary;
typedef struct _Parameter_Info Parameter_Info;

////////////////////////////////////////////////////////////////////////////////
// Global tables.
////////////////////////////////////////////////////////////////////////////////

/*
 * Tables to contain information about the instructions, symbols and variables
 * that is obtained from the assembler file.
 */
extern Symbol_Info *sym_tbl;
extern Instruction_Info *inst_tbl;
extern Variable_Info *var_tbl;

/*
 * Specifies the size of the tables.
 */
extern uint32_t sym_cnt;
extern uint32_t inst_cnt;
extern uint32_t var_cnt;

#endif /* GLOBAL_H_ */
