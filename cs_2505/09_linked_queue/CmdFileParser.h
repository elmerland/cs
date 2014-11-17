#ifndef CMDFILEPARSER_H
#define CMDFILEPARSER_H

#define MAXCMDLENGTH 10
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

struct _Command {
   char    word[MAXCMDLENGTH];    // string identifying command
   int32_t parameter;             // parameter to command
};

typedef struct _Command Command;

// Installs name of file to read commands from.
//
// Pre:  fName points to C-string holding name of file
// Post: file name is stored statically in CmdFileParse.c
// Returns true if file is opened successfully, false otherwise
//
bool    setCmdFile(char* fName);

// Closes file holding commands.
//
// Pre:  file has been set (and opened) by a call to setCmdFile()
// Post: file has been closed
//
void    closeCmdFile();

// Retrieves and returns next command in file.
//
// Pre:  file holding commands has been set and opened by a call to setCmdFile()
// Returns Command object representing next command in file, if any
//
Command getNextCommand();

#endif
