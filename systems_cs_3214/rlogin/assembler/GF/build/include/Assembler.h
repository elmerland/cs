/*
 * Assembler.h
 * 	This class contains the main methods that runs the entire program. It in
 * 	charge of calling all the other appropriate functions to parse the input
 * 	file into the desired output file.
 *
 *  Created on: Oct 20, 2013
 *      Author: Elmer Landaverde
 */
// Assignment 4: Assembler. CS 2506 Fall 2013
#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include "StructUtilities.h"
#include "Preprocessor.h"
#include "Parser.h"
#include "Translator.h"
#include "OutputManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#define MAX_ARGS 4
#define MIN_ARGS 3

// File pointers
extern FILE *input;
extern FILE *output;
extern FILE *clean_file;

// Boolean variables for the arguments flags.
extern bool has_options;
extern bool symbols_option;
extern bool list_option;

// Various strings needed for comparison.
extern char *symbols_string_flag;
extern char *list_string_flag;
extern char *clean_file_name;

#endif/* ASSEMBLER_H_ */
