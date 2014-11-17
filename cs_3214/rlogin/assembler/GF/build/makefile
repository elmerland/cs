#	Makefile for assignment 4: Assembler. CS 2506 Fall 2013
#	Name:	Elmer Landaver
#	email:	elmer@vt.edu
#	id:		9054-91691

#	Set compilation directories.
#	SDIR	The directory of all the .c files.
#	IDIR	The directory of all the .h files.
#	ODIR	The directory where all the .o files will be written to.
SDIR =./src
IDIR =./include
ODIR =.

#	Set compiler.
CC=gcc

#	Set compilation options
#
#	-O0			No optimization (Remove after debugging).
#	-m32		Create a 32-bit executable.
#	-std=c99	Use C99 standard features.
#	-Wall		Show "all" warnings.
#	-W			Show even more warnings (Annoying).
CFLAGS=-O0 -m32 -std=c99 -Wall -W

#	Set include file path.
CINCLUDE=-I$(IDIR)

#	Set libraries to be included.


FILES = Assembler BinaryUtilities OutputManager Parser Preprocessor StructUtilities Tables Translator Utilities

#	Create list of header files.
DEPS = $(patsubst %,$(IDIR)/%.h,$(FILES))

#	Create list of object files.
OBJ = $(patsubst %,$(ODIR)/%.o,$(FILES))

#	Create object files.
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(CINCLUDE)

#	Create executable file.
assembler: $(OBJ)
	$(CC) $(CFLAGS) -o assemble $^ $(CINCLUDE) $(LIBS)

#	Clean up
.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(INCDIR)/*.o *.o *~ $(ODIR)/*~ $(INCDIR)/*~ 

