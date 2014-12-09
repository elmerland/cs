# Makefile for P4/Q1 - OpenMPI, CS 4234, Fall 2014
#
# Scott Sines

GCC = gcc
MCC = mpicc
CFLAGS = -Wall -pthread -fopenmp
HOSTFILE = ../rlogin_nodes
EXEC_1 = prob1
# MPI_PARAMS = -np 2 -map-by ppr:1:node -hostfile $(HOSTFILE)

.PHONY: all compile run clean

# Build full project:

all: compile run

compile: prob1.c
	$(MCC) $(CFLAGS) -o $(EXEC_1) prob1.c

run1a:
	./fullcheck.sh

gen: gen.c
	$(GCC) $(CFLAGS) -o gen gen.c

# Cleaning rule:
clean:
	rm -f $(EXEC_1)
