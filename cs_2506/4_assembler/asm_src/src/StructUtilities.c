/*
 * StructUtilities.c
 *	This source file contains several methods that help in the creation,
 *	allocation and deallocation of resources for structures.
 *
 *  Created on: Oct 20, 2013
 *      Author: Elmer Landaverde
 */

#include "StructUtilities.h"

void set_inst_inf_a(Instruction_Info instructions[], uint32_t size) {
	for (uint32_t i = 0; i < size; i++) {
		set_inst_inf(&instructions[i]);
	}
}

void set_inst_inf(Instruction_Info *instruction) {
	instruction->addr = 0;
	instruction->inst = NULL;
	instruction->line = NULL;
	instruction->par1 = NULL;
	instruction->par2 = NULL;
	instruction->par3 = NULL;
	instruction->type = I_NOT_DEF;
	instruction->binary = NULL;
}

void print_inst_inf_a(Instruction_Info instructions[], uint32_t size) {
	for (uint32_t i = 0; i < size; i++) {
		print_inst_inf(&instructions[i]);
	}
}

void print_inst_inf(Instruction_Info *instruction) {
	printf(">> Instruction Info:\n");
	// Original line.
	printf("\tLine:\t%s", instruction->line);
	// Address
	printf("\tAddr:\t0x%"PRIX32"\t", instruction->addr);
	// Instruction name.
	printf("\tName:\t%s\n", instruction->inst);
	// Parameter 1
	if (instruction->par1 == NULL) {
		printf("\tPar 1:\tNONE\t");
	} else {
		printf("\tPar 1:\t%s\t", instruction->par1);
	}
	// Parameter 2.
	if (instruction->par2 == NULL) {
		printf("\tPar 2:\tNONE\t");
	} else {
		printf("\tPar 2:\t%s\t", instruction->par2);
	}
	// Parameter 3
	if (instruction->par3 == NULL) {
		printf("\tPar 3:\tNONE\n");
	} else {
		printf("\tPar 3:\t%s\n", instruction->par3);
	}
	// Instruction type.
	if (instruction->type == R_TYPE) {
		printf("\tType:\tR_TYPE\n");
	} else if (instruction->type == I_TYPE) {
		printf("\tType:\tI_TYPE\n");
	} else if (instruction->type == J_TYPE) {
		printf("\tType:\tJ_TYPE\n");
	} else {
		printf("\tType:\tNOT_DEF\n");
	}
	// Binary
	if (instruction->binary == NULL) {
		printf("\tBinary:\tNULL\n");
	} else {
		printf("\tBinary:\t%s\n", instruction->binary);
	}
}

void free_inst_inf_a(Instruction_Info instruction[], uint32_t size) {
	for (uint32_t i = 0; i < size; i++) {
		free_inst_inf(&instruction[i]);
	}
}

void free_inst_inf(Instruction_Info *instruction) {
	if (instruction->inst != NULL) {
		free(instruction->inst);
	}
	if (instruction->line != NULL) {
		free(instruction->line);
	}
	if (instruction->par1 != NULL) {
		free(instruction->par1);
	}
	if (instruction->par2 != NULL) {
		free(instruction->par2);
	}
	if (instruction->par3 != NULL) {
		free(instruction->par3);
	}
	if (instruction->binary != NULL) {
		free(instruction->binary);
	}
}

void set_sym_inf_a(Symbol_Info symbols[], uint32_t size) {
	for (uint32_t i = 0; i < size; i++) {
		set_sym_inf(&symbols[i]);
	}
}

void set_sym_inf(Symbol_Info *symbol) {
	symbol->addr = 0;
	symbol->symbol = NULL;
}

void print_sym_inf_a(Symbol_Info symbols[], uint32_t size) {
	for (uint32_t i = 0; i < size; i++) {
		print_sym_inf(&symbols[i]);
	}
}

void print_sym_inf(Symbol_Info *symbol) {
	printf(">> Symbol Info:\n");
	printf("\tAddr:\t0x%.8"PRIX32"\n", symbol->addr);
	if (symbol->symbol == NULL) {
		printf("\tSymbol:\tNULL\n");
	} else {
		printf("\tSymbol:\t%s\n", symbol->symbol);
	}
}

void free_sym_inf_a(Symbol_Info symbols[], uint32_t size) {
	for (uint32_t i = 0; i < size; i++) {
		free_sym_inf(&symbols[i]);
	}
}

void free_sym_inf(Symbol_Info *symbol) {
	if (symbol->symbol != NULL) {
		free(symbol->symbol);
	}
}

void set_var_inf_a(Variable_Info vars[], uint32_t size) {
	for (uint32_t i = 0; i < size; i++) {
		set_var_inf(&vars[i]);
	}
}

void set_var_inf(Variable_Info *var) {
	var->addr = 0;
	var->label = NULL;
	var->line = NULL;
	var->name = NULL;
	var->parameter = NULL;
	var->length = 0;
	var->byte_length = 0;
	var->type = V_NOT_DEF;
}

void print_var_inf_a(Variable_Info vars[], uint32_t size) {
	for (uint32_t i = 0; i < size; i++) {
		print_var_inf(&vars[i]);
	}
}

void print_var_inf(Variable_Info *var) {
	printf(">> Variable Info:\n");
	// Address
	printf("\tAddress:\t0x%"PRIX32"\n", var->addr);
	// Label
	if (var->label == NULL) {
		printf("\tLabel:\tNULL\n");
	} else {
		printf("\tLabel:\t%s\n", var->label);
	}
	// Line
	if (var->line == NULL) {
		printf("\tLine:\tNULL\n");
	} else {
		printf("\tLine:\t%s", var->line);
	}
	// Name
	if (var->name == NULL) {
		printf("\tName:\tNULL\n");
	} else {
		printf("\tName:\t%s\n", var->name);
	}
	// Parameter
	if (var->parameter == NULL) {
		printf("\tParameter:\tNULL\n");
	} else {
		printf("\tParameter:\t<%s>\n", var->parameter);
	}
	printf("\tLength:\t0x%"PRIX32"\n", var->length);
	printf("\tBytes:\t0x%.8"PRIX32"\n", var->byte_length);
	// Type
	if (var->type == WORD) {
		printf("\tType:\tWORD\n");
	} else if (var->type == WORD_A) {
		printf("\tType:\tWORD_A\n");
	} else if (var->type == ASCII) {
		printf("\tType:\tASCII\n");
	} else {
		printf("\tType:\tV_NOT_DEF\n");
	}

}

void free_var_inf_a(Variable_Info vars[], uint32_t size) {
	for (uint32_t i = 0; i < size; i++) {
		free_var_inf(&vars[i]);
	}
}

void free_var_inf(Variable_Info *var) {
	if (var->label != NULL) {
		free(var->label);
	}
	if (var->line != NULL) {
		free(var->line);
	}
	if (var->name != NULL) {
		free(var->name);
	}
	if (var->parameter != NULL) {
		free(var->parameter);
	}
}

void free_parser_arrays() {
	printf("> Deallocating memory ... ");
	if (sym_tbl != NULL) {
		free_sym_inf_a(sym_tbl, sym_cnt);
		free(sym_tbl);
	}
	if (inst_tbl != NULL) {
		free_inst_inf_a(inst_tbl, inst_cnt);
		free(inst_tbl);
	}
	if (var_tbl != NULL) {
		free_var_inf_a(var_tbl, var_cnt);
		free(var_tbl);
	}
	printf("Done\n");
}
