/*
 * BinaryUtilities.h
 * 	Contains several functions that parse between integer or string to their
 * 	binary representation and other functionality.
 *
 *  Created on: Nov 14, 2013
 *      Author: Elmer Landaverde
 */

#ifndef BINARYUTILITIES_H_
#define BINARYUTILITIES_H_

#include "Global.h"
#include "Tables.h"
#include "Utilities.h"

#include <stdlib.h>
#include <stdbool.h>

/**
 *	write_to_bin() This function writes the given string to the binary line
 *				string at the position specified by the parameter structure.
 *	Pre:		The token points to a properly initialized null terminated
 *				string array.
 *				The parameter structure is properly initialized and contains the
 *				starting point of the parameter and size of it.
 *				The bin_line is a properly initialized string array.
 *	Post:		The token has being written to the binary line at the location
 *				specified by the parameter structure.
 *	Return:		True if the token was written to the binary line successfully.
 *				Otherwise it returns false.
 */
bool write_to_bin(const char * const token,
		const Parameter_Info * const par, char * const bin_line);

/**
 *	write_num_to_bin() This is a helper function that takes in a number and
 *				translates this number into a binary string representation. This
 *				binary string representation however, only reads the 5 rightmost
 *				bits. Furthermore, only the leftmost 'par->size' bits will be
 *				written to the binary line.
 *	Pre:		The parameter structure, and the binary line follow the same
 *				pre-conditions as the write_to_bin() function.
 *				The number is an unsigned 32 bit integer.
 *	Post:		The number has being written to the binary line at the location
 *				specified by the parameter structure.
 *	Return:		True if the number was written to the binary line successfully.
 *				Otherwise it returns false.
 */
bool write_num_to_bin(uint32_t number, const Parameter_Info * const par,
		char * const bin_line);

/**
 *	write_num_to_bin_16() This is a helper function that takes in a number and
 *				translates this number into a binary string representation. This
 *				representation only writes the leftmost 'par->size' bits to the
 *				binary line. NOTE: This function takes in a signed integer.
 *	Pre:		The parameter structure, and the binary line follow the same
 *				pre-conditions as the write_to_bin() function.
 *				The number is an unsigned 16 bit integer.
 *	Post:		The number has being written to the binary line at the location
 *				specified by the parameter structure.
 *	Return:		True if the number was written to the binary line successfully.
 *				Otherwise it returns false.
 */
bool write_num_to_bin_16(uint16_t number,
		const Parameter_Info * const par, char * const bin_line);

/**
 *	write_num_to_bin_16_s() This is a helper function that takes in a number and
 *				translates this number into a binary string representation. This
 *				representation only writes the leftmost 'par->size' bits to the
 *				binary line. NOTE: This function takes in an unsigned integer.
 *	Pre:		The parameter structure, and the binary line follow the same
 *				pre-conditions as the write_to_bin() function.
 *				The number is an unsigned 16 bit integer.
 *	Post:		The number has being written to the binary line at the location
 *				specified by the parameter structure.
 *	Return:		True if the number was written to the binary line successfully.
 *				Otherwise it returns false.
 */
bool write_num_to_bin_16_s(int16_t number,
		const Parameter_Info * const par, char * const bin_line);



/**
 *	write_register() This function writes the specified register to the binary
 *				line at the location specified by the parameter structure.
 *	Pre:		The register is a properly initialized string that matches one
 *				of the 32 available registers.
 *				The line is a properly initialized null terminated string.
 *				The parameter structure is properly initialized.
 *	Post:		The line has being modified to contain the binary representation
 *				of the register.
 *	Return:		True if the register could be written to the binary line.
 *				Otherwise it returns null.
 */
bool write_register(const char * const reg,
		const Parameter_Info * const par, char * const line);

/**
 *	get_bin_number() This function takes in a string that contains a number. It
 *				then parses this string number into a string binary
 *				representation of the number.
 *	Pre:		Num contains a valid integer value.
 *				Bin_number is a properly initialized string of length 33.
 *	Post:		Bin_number contains the binary representation of the integer.
 *				Num remains unchanged.
 */
void get_bin_number(const char * const num, char * const bin_number);

/**
 *	allocate_bin() This function allocates memory to the 'bin' field of the
 *				instruction structure. It will allocate enough space for a 33
 *				character string and sets every character to a null terminator.
 *	Pre:		The instruction structure contains a field named 'bin' that is
 *				set to NULL and has not being allocated any memory.
 *	Post:		The 'bin' field in the instruction structure points to a string
 *				of length 33, with all the characters set to a null terminator.
 *	Return:		True if the memory could be allocated. Otherwise it returns
 *				false.
 */bool allocate_bin(Instruction_Info * const inst);

#endif /* BINARYUTILITIES_H_ */
