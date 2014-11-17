/*
 * OutputManager.h
 * 	This file is in charge of printing the required information to the ouput
 * 	file in the specified format.
 *
 *  Created on: Oct 23, 2013
 *      Author: Elmer Landaverde
 */

#ifndef OUTPUTMANAGER_H_
#define OUTPUTMANAGER_H_

#include "Global.h"
#include "Utilities.h"
#include "BinaryUtilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

/**
 *	print_output() This function prints out the results of the translator and
 *				parser to the specified output file.
 *	Pre:		List option and Symbols option determine if the list and symbols
 *				options are turned on.
 *				Output points to a writable file.
 *				The symbols, variable and instruction tables are complete.
 *	Post:		The output file contains all the information specified by the
 *				options.
 */
void print_output(bool listOption, bool symbolsOption, FILE *output);

#endif /* OUTPUTMANAGER_H_ */
