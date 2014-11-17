/*
 * Parser.h
 * 	This file is in charge of extracting all the relevant information from the
 * 	clean file and filling out the tables that will be used by the translator.
 *
 * 
 *  Created on: Oct 20, 2013
 *      Author: Elmer Landaverde
 */

#ifndef ASM_TEXT_PARSER_H_
#define ASM_TEXT_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

/**
 *	parse() This function takes in a cleaned asm file and every single line in
 *				it logical parts. For every instruction it creates an
 *				Instruction_Info structure, for every symbol it creates a
 *				Symbol_Info structure, and for every variable it creates a
 *				Variable_Info structure.
 *	Pre:		Input points to a readable file.
 *	Post:		All the tables declared in this header file have being properly
 *				initialized and filled with all the relevant information from
 *				the input file.
 *	Return:		True if all the information was parsed successfully. Returns
 *				false if there are unknown instructions, or incorrect syntax.
 */
bool parse(FILE *input);

#endif/* ASMTEXTPARSER_H_ */
