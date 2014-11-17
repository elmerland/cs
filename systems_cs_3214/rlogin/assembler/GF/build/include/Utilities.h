/*
 * Utilities.h
 * 	This file contains miscellaneous functions that are used throught the
 * 	program.
 *
 *  Created on: Nov 7, 2013
 *      Author: Elmer Landaverde
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "Global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <ctype.h>

/*
 * Helper boolean for the get_symbol_addr() function.
 */
extern bool has_error;

/**
 *	parse_int() This function takes a string of characters and turns it into an
 *				unsigned 32 bit integer.
 *	Pre:		Number points to a properly initialized null terminated string
 *				that only contains digits.
 *	Post:		The number string remains unchanged.
 *	Return:		The integer representation of the given number string.
 */
uint32_t parse_int(const char * const number);

/**
 *	parse_int_s() This function takes a string of characters and turns it into a
 *				signed 32 bit integer.
 *	Pre:		Number points to a properly initialized null terminated string
 *				that only contains digits.
 *	Post:		The number string remains unchanged.
 *	Return:		The integer representation of the given number string.
 */
int32_t parse_int_s(const char * const number);

/**
 *	parse_int_16() This function takes a string of characters and turns it into
 *				an unsigned 16 bit integer.
 *	Pre:		Number points to a properly initialized null terminated string
 *				that only contains digits.
 *	Post:		The number string remains unchanged.
 *	Return:		The integer representation of the given number string.
 */
uint16_t parse_int_16(const char * const number);

/**
 *	parse_int_s_16() This function takes a string of characters and turns it
 *				into a signed 16 bit integer.
 *	Pre:		Number points to a properly initialized null terminated string
 *				that only contains digits.
 *	Post:		The number string remains unchanged.
 *	Return:		The integer representation of the given number string.
 */
int16_t parse_int_s_16(const char * const number);

/**
 *	Print_Text_File() This function prints the contents of the given file to
 *				console.
 *	Pre:		The file pointer points to a properly initialized file pointer.
 *	Post:		The file pointer has not changed, and the contents of it have
 *				being output to the console.
 * Called by:	Generate_Binary().
 */
void prnt_txt_file(FILE *file);

/**
 *	print_bin_chars() This function takes in four characters and prints them out
 *				to the specified file in reverse order.
 *	Pre:		All characters are valid.
 *				The file pointer is writable.
 *	Post:		The file pointer is advanced to a new line.
 */
void print_bin_chars(FILE *output, const char c1, const char c2, const char c3,
		const char c4);

/**
 *	print_bin_char() This function prints out the given character to the
 *				specified file.
 *	Pre:		The character is valid.
 *				The file pointer is writable.
 *	Post:		The character remains unchanged.
 *				The file pointer has advanced to where the character was written
 *				to.
 */
void print_bin_char(FILE *output, char c);

/**
 *	print_label() This function prints the symbol at the specified address to
 *				the specified file. The add_tabs boolean specified if the
 *				label should be printed with an offset of 12 tabs.
 *	Pre:		Addr corresponds to a valid symbol, that is contained in the
 *				symbol table.
 *				The file pointer is writable.
 *	Post:		The file pointer has being advance to a new line.
 */
void print_label(uint32_t addr, FILE *output, bool add_tabs);

/**
 *	print_address() This function prints the given address to the specified
 *				file, in hex-decimal format.
 *	Pre:		The file pointer is writable.
 *				The address is valid.
 *	Post:		The file pointer has being advanced to the end of the address
 *				that was just written.
 */
void print_address(FILE *output, uint32_t addr);

/**
 *	get_symbol_addr() This function takes in the name of a label (Symbol) and
 *				determines the address of that label by looking it up in the
 *				symbols table. If the symbol does not match any valid label then
 *				the has_error boolean will be set to true. Otherwise it will be
 *				set to false.
 *	Pre:		Symbol is a properly initialized null terminated string.
 *	Post:		Symbol remains unchanged.
 *	Return:		A 32 bit unsigned integer that represents the memory address of
 *				the label.
 */
uint32_t get_symbol_addr(const char * const symbol);

/**
 *	is_symbol() This function determines if the symbol string is a valid symbol
 *				name.
 *	Pre:		The symbol is a properly initialized null terminated string.
 *	Post:		The symbol remains unchanged.
 *	Return:		True if the given symbol is a valid symbol name, otherwise it
 *				returns false.
 */bool is_symbol(const char * const symbol);

/**
 *	get_index() This function takes an array of dictionary structures and looks
 *				for a key value that matches 'name'. If such a value is found,
 *				it will return the index position of that value in the array.
 *	Pre:		The table points to a properly initialized array of dictionary
 *				structures.
 *				The size indicates the size of the array.
 *				The name is a properly initialized string with a null terminator
 *				of maximum length TKN_LEN.
 *	Post:		The table, and name remain unchanged.
 *	Return:		The index of the matching name entry in the provided array.
 *				If no match is found, it returns -1.
 */
int32_t get_index(const char * const name, const Dictionary table[],
		uint32_t size);

/**
 *	contains() This functions checks if the given array of strings contains a
 *				string that matches the 'name'. If such a value is found it
 *				returns true.
 *	Pre:		The name is a properly initialized null terminating string.
 *				The array list is a properly initialized array of strings.
 *	Post:		The name and list remained unchanged.
 *	Return:		True if the list contains a value that matches 'name'. Otherwise
 *				it returns false.
 */bool contains(const char * const name, const char * const list[], int size);

 /**
  *	Clean_Label() This function removed the ':' at the end of a label. The
  *				resulting string is stored back to the given 'line'.
  *	Pre:		The line string points to a properly initialized string of
  *				maximum length MAX_LN_LEN.
  *	Post:		The line string has being modified to remove the ':' character
  *				and anything after that (Including new line characters).
  *	Returns:	True if the label could be cleaned up. False if the 'line'
  *				pointer is null.
  *	Called by:	AsmParser.Get_Labels().
  */
 bool clean_lbl(char *line);

 /**
  *	Has_Label() This function determines if the given line of assembly code
  *				has a label. It determines this by finding a ':' character
  *				before the first space character.
  *	Pre:		Line points to a valid line of assembly code.
  *	Post:		Line remains unchanged.
  *	Returns:	True if the line of assembly code contains a label, returns
  *				false otherwise.
  */
 bool has_lbl(const char * const line);

#endif /* NUMUTILITIES_H_ */
