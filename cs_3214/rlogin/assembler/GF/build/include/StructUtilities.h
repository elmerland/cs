/*
 * StructUtilities.h
 * 	This file contains miscellaneous functions that deal with the creation,
 * 	allocation and deallocation of structures.
 *
 *  Created on: Oct 20, 2013
 *      Author: Elmer Landaverde
 */

#ifndef STRUCT_UTILITIES_H_
#define STRUCT_UTILITIES_H_

#include "Global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

/*
 * The following functions are meant to help initialize, visualize and
 * deallocate memory for all Instruction_Info structures.
 */

void set_inst_inf_a(Instruction_Info instructions[], uint32_t size);

void set_inst_inf(Instruction_Info *instruction);

void print_inst_inf_a(Instruction_Info instructions[], uint32_t size);

void print_inst_inf(Instruction_Info *instruction);

void free_inst_inf_a(Instruction_Info instruction[], uint32_t size);

void free_inst_inf(Instruction_Info *instruction);

/*
 * The following functions are meant to help initialize, visualize and
 * deallocate memory for all Symbol_Info structures.
 */

void set_sym_inf_a(Symbol_Info symbols[], uint32_t size);

void set_sym_inf(Symbol_Info *symbol);

void print_sym_inf_a(Symbol_Info symbols[], uint32_t size);

void print_sym_inf(Symbol_Info *symbol);

void free_sym_inf_a(Symbol_Info symbols[], uint32_t size);

void free_sym_inf(Symbol_Info *symbol);

/*
 * The following functions are meant to help initialize, visualize and
 * deallocate memory for all Variable_Info structures.
 */

void set_var_inf_a(Variable_Info vars[], uint32_t size);

void set_var_inf(Variable_Info *var);

void print_var_inf_a(Variable_Info vars[], uint32_t size);

void print_var_inf(Variable_Info *var);

void free_var_inf_a(Variable_Info vars[], uint32_t size);

void free_var_inf(Variable_Info *var);

/**
 *	free_arrays() This function makes sure that all the memory allocated to any
 *				structure has being deallocated properly.
 *	Pre:		Sym_tbl points to a properly initialized structure array.
 *				Inst_tbl points to a properly initialized structure array.
 *				Var_tbl points to a properly initialized structure array.
 *	Post:		Each of the string variables in each of the structures have
 *				being deallocated.
 */
void free_parser_arrays();

#endif/* UTILITIES_H_ */
