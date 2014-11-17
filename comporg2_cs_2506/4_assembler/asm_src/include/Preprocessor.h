/*
 * Preprocessor.h
 * 	This file is in charge cleaning the input file and discarding any
 * 	non-relevant information.
 *
 *  Created on: Oct 20, 2013
 *      Author: Elmer Landaverde
 */

#ifndef PREPROCESSOR_H_
#define PREPROCESSOR_H_

#include "Global.h"
#include "Utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

/**
 *	Preprocess_File() This function takes the text from the input file and
 *				discards any irrelevant content. Irrelevant content means
 *				blank lines; leading or trailing spaces, tabs or commas; and
 *				comments at the start or at the end of a line. The
 *				The non-irrelevant text is then copied to the output file.
 *	Pre:		input points to a valid text file.
 *				output points to a valid text file.
 *	Post:		input has not changed.
 *				output contains all non-irrelevant text contained in the input file.
 *				Both pointer for input and output point to the start of the file.
 *	Returns:	The total number of lines copied to the output file. Returns -1
 *				if either of the file pointers are null or if the input file
 *				could not be parsed.
 *	Called by:	Generate_Binary().
 */
uint32_t preprocess(FILE *input, FILE *output);

#endif /* FILECLEANER_H_ */
