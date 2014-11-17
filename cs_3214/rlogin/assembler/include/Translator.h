/*
 * Translator.h
 * 	This file is charge of generating the binary representation of all the
 * 	assembly instructions.
 *
 *  Created on: Oct 26, 2013
 *      Author: Elmer Landaverde
 */

#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

#include "Global.h"
#include "Utilities.h"
#include "BinaryUtilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

/**
 *	translate() This function translates all the instructions from the input
 *				file into binary code. This function takes its data from the
 *				instructions, symbols and variables tables that the parsers
 *				generated.
 *	Pre:		The instructions, symbols, and variables tables have being
 *				properly initialized and completely filled. All sudo
 *				instructions have already being translated to real code.
 *	Post:		The 'bin' field inside of the instruction structure has being
 *				filled out with the corresponding binary code for the
 *				corresponding address. Every binary line will be of length 33,
 *				where the last character is a null terminator and the rest of
 *				the characters are either 1's or 0's.
 *	Return:		True if the assembler was translated successfully. Returns false
 *				otherwise. This function may return false if the assembly code
 *				has unknown or invalid functions. If the parameters of the
 *				function are not correct, the behavior of this function is
 *				undefined.
 */
bool translate();

#endif /* TRANSLATOR_H_ */
