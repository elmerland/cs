.PHONY: compile, run

compile: prob8.c
	gcc -Wall -Werror -fopenmp -o prob8 prob8.c list.c
	mpicc -Wall -Werror -fopenmp -o prob9 prob9.c list.c
	mpicc -Wall -Werror -pthread -o prob10 prob10.c list.c

run:
	mpirun -np 4 -hostfile ./rlogin_nodes ./prob9

run1:
	mpirun -np 4 -hostfile ./rlogin_nodes ./prob10