/*
 * Tables.h
 * 	This file declares several tables and arrays that are used by the translator
 * 	to decode information like registers and paramter positions.
 *
 *  Created on: Oct 20, 2013
 *      Author: Elmer Landaverde
 */
#ifndef TABLES_H_
#define TABLES_H_

#include "Global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

/*
 * Templates for all the positions of all types of parameters.
 */
extern const Parameter_Info OPC;	// Opcode parameter
extern const Parameter_Info RS;		// rs parameter
extern const Parameter_Info RT;		// rt parameter
extern const Parameter_Info RD;		// rd parameter
extern const Parameter_Info SA;		// sa parameter
extern const Parameter_Info FUNC;	// Function code parameter
extern const Parameter_Info IMM;	// 16-bit Immediate parameter
extern const Parameter_Info BASE;	// Base parameter
extern const Parameter_Info OFF;	// Offset parameter
extern const Parameter_Info TARZ;	// Zeroed out portion of target address
extern const Parameter_Info TARA;	// 16-bit target address
extern const Parameter_Info JRZ;	// Zero bits for the jr instruction
extern const Parameter_Info SYS;	// Zero bits for the syscall instruction

/*
 * Useful constants for filling out zeros in a binary line.
 */
extern const char * const ZERO_TOKEN;	// 5 zeros
extern const char * const ZERO_REG;		// $zero
extern const char * const ZERO_CODE;	// 20 zeros
extern const char * const ZERO_10;		// 10 zeros
extern const char * const ZERO_15;		// 15 zeros

/*
 * Tables that hold values for function codes, registers and opcodes.
 */
extern const Dictionary REG_TBL[];
extern const Dictionary OPC_TBL[];
extern const Dictionary FUNC_TBL[];

/*
 * The sizes of all the dictionary tables.
 */
extern const uint32_t REG_TBL_SIZE;
extern const uint32_t OPC_TBL_SIZE;
extern const uint32_t FUNC_TBL_SIZE;

#endif /* TYPES_H_ */
