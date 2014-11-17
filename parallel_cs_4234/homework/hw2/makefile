.PHONY: all compile run

all: compile run

compile: prob2 prob3a prob3b

run: run_prob2 run_prob3

prob2: prob2.c
	mpicc -Wall -o prob2 prob2.c

prob3a: prob3a.c
	mpicc -Wall -o prob3a prob3a.c

prob3b: prob3b.c
	mpicc -Wall -o prob3b prob3b.c

run_prob2:
	mpirun -np 64 -hostfile ../nodes ./prob2

run_prob3: run_prob3a run_prob3b

run_prob3a:
	mpirun -np 8 -hostfile ../nodes ./prob3a

run_prob3b:
	mpirun -np 4 -hostfile ../nodes ./prob3b
