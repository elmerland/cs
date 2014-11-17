/*
 * Tables.c
 *	This source file creates some tables that are used when parsing parameters
 *	to binary.
 *
 *  Created on: Oct 24, 2013
 *      Author: Elmer Landaverde
 */
#include "Tables.h"

/*
 * Constants for filling out zero's in the binary lines.
 */
const char * const ZERO_TOKEN = "00000";
const char * const ZERO_REG = "$zero";
const char * const ZERO_CODE = "00000000000000000000";
const char * const ZERO_10 = "0000000000";
const char * const ZERO_15 = "000000000000000";

/*
 * Register table.
 */
const Dictionary REG_TBL[32] = { { "$zero", "00000" }, // 01
		{ "$at", "00001" }, // 02
		{ "$v0", "00010" }, // 03
		{ "$v1", "00011" }, // 04
		{ "$a0", "00100" }, // 05
		{ "$a1", "00101" }, // 06
		{ "$a2", "00110" }, // 07
		{ "$a3", "00111" }, // 08
		{ "$t0", "01000" }, // 09
		{ "$t1", "01001" }, // 10
		{ "$t2", "01010" }, // 11
		{ "$t3", "01011" }, // 12
		{ "$t4", "01100" }, // 13
		{ "$t5", "01101" }, // 14
		{ "$t6", "01110" }, // 15
		{ "$t7", "01111" }, // 16
		{ "$s0", "10000" }, // 17
		{ "$s1", "10001" }, // 18
		{ "$s2", "10010" }, // 19
		{ "$s3", "10011" }, // 20
		{ "$s4", "10100" }, // 21
		{ "$s5", "10101" }, // 22
		{ "$s6", "10110" }, // 23
		{ "$s7", "10111" }, // 24
		{ "$t8", "11000" }, // 25
		{ "$t9", "11001" }, // 26
		{ "$k0", "11010" }, // 27
		{ "$k1", "11011" }, // 28
		{ "$gp", "11100" }, // 29
		{ "$sp", "11101" }, // 20
		{ "$fp", "11110" }, // 31
		{ "$ra", "11111" } // 32
};
/*
 * Size of register table.
 */
const uint32_t REG_TBL_SIZE = 32;

/*
 * Opcode table.
 */
const Dictionary OPC_TBL[] = { { "lw", "100011" }, // 1
		{ "sw", "101011" }, // 2
		{ "add", "000000" }, // 3
		{ "sub", "000000" }, // 4
		{ "addi", "001000" }, // 5
		{ "addiu", "001001" }, // 6
		{ "or", "000000" }, // 7
		{ "and", "000000" }, // 8
		{ "ori", "001101" }, // 9
		{ "andi", "001100" }, // 10
		{ "slt", "000000" }, // 11
		{ "slti", "001010" }, // 12
		{ "sll", "000000" }, // 13
		{ "srl", "000000" }, // 14
		{ "sra", "000000" }, // 15
		{ "beq", "000100" }, // 16
		{ "bne", "000101" }, // 17
		{ "bltz", "000001" }, // 18
		{ "blez", "000110" }, // 19
		{ "j", "000010" }, // 20
		{ "jr", "000000" }, // 21
		{ "jal", "000011" }, // 22
		{ "syscall", "000000" } // 23
};
/*
 * Size of opcode table.
 */
const uint32_t OPC_TBL_SIZE = 23;

/*
 * Function codes table.
 */
const Dictionary FUNC_TBL[] = { { "add", "100000" }, // 1
		{ "sub", "100010" }, // 2
		{ "or", "100101" }, // 3
		{ "and", "100100" }, // 4
		{ "slt", "101010" }, // 5
		{ "sll", "000000" }, // 6
		{ "srl", "000010" }, // 7
		{ "sra", "000011" }, // 8
		{ "jr", "001000" }, // 9
		{ "syscall", "001100" } // 10
};
/*
 * Size of function table.
 */
const uint32_t FUNC_TBL_SIZE = 10;

/*
 * Structures that hold the position of parameters. It stores the starting
 * position of the paramter and the length in bits.
 * Example: {31, 6} = starts at bit number 31 (first bit) and ends at bit 26
 * (inclusive).
 */
const Parameter_Info OPC = { 31, 6 };
const Parameter_Info RS = { 25, 5 };
const Parameter_Info RT = { 20, 5 };
const Parameter_Info RD = { 15, 5 };
const Parameter_Info SA = { 10, 5 };
const Parameter_Info FUNC = { 5, 6 };
const Parameter_Info IMM = { 15, 16 };
const Parameter_Info BASE = { 25, 5 };
const Parameter_Info OFF = { 15, 16 };
const Parameter_Info TARZ = { 25, 10 };
const Parameter_Info TARA = { 15, 16 };
const Parameter_Info JRZ = { 20, 15 };
const Parameter_Info SYS = { 25, 20 };
