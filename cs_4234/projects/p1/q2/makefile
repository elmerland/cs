.PHONY: all compile run

all: compile run

compile: q2a_ag q2a_g q2b_ag q2b_g q2c

runa: run2a_ag run2a_g

runb: run2b_ag run2b_g

runc: run2c

# Compile file for quetion 2a - all gather
q2a_ag: q2a_ag.c
	mpicc -o q2a_ag q2a_ag.c

# Compile file for quetion 2a - gather
q2a_g: q2a_g.c
	mpicc -o q2a_g q2a_g.c

# Compile file for quetion 2b - all gather
q2b_ag: q2b_ag.c
	mpicc -o q2b_ag q2b_ag.c

# Compile file for quetion 2b - gather
q2b_g: q2b_g.c
	mpicc -o q2b_g q2b_g.c

# Compile file for quetion 2c
q2c: q2c.c
	mpicc -o q2c q2c.c

run2a_ag:
	@echo ", All gather 4"
	mpirun -np 4 -npernode 4 -hostfile ../nodes ./q2a_ag
	@echo ", All gather 8"
	mpirun -np 8 -npernode 4 -hostfile ../nodes ./q2a_ag
	@echo ", All gather 16"
	mpirun -np 16 -npernode 4 -hostfile ../nodes ./q2a_ag
	@echo ", All gather 24"
	mpirun -np 24 -npernode 4 -hostfile ../nodes ./q2a_ag

run2a_g:
	@echo ", Gather 4"
	mpirun -np 4 -npernode 4 -hostfile ../nodes ./q2a_g
	@echo ", Gather 8"
	mpirun -np 8 -npernode 4 -hostfile ../nodes ./q2a_g
	@echo ", Gather 16"
	mpirun -np 16 -npernode 4 -hostfile ../nodes ./q2a_g
	@echo ", Gather 24"
	mpirun -np 24 -npernode 4 -hostfile ../nodes ./q2a_g

run2b_ag:
	@echo ", All gather 4"
	mpirun -np 4 -npernode 4 -hostfile ../nodes ./q2b_ag
	@echo ", All gather 8"
	mpirun -np 8 -npernode 4 -hostfile ../nodes ./q2b_ag
	@echo ", All gather 16"
	mpirun -np 16 -npernode 4 -hostfile ../nodes ./q2b_ag
	@echo ", All gather 24"
	mpirun -np 24 -npernode 4 -hostfile ../nodes ./q2b_ag

run2b_g:
	@echo ", Gather 4"
	mpirun -np 4 -npernode 4 -hostfile ../nodes ./q2b_g
	@echo ", Gather 8"
	mpirun -np 8 -npernode 4 -hostfile ../nodes ./q2b_g
	@echo ", Gather 16"
	mpirun -np 16 -npernode 4 -hostfile ../nodes ./q2b_g
	@echo ", Gather 24"
	mpirun -np 24 -npernode 4 -hostfile ../nodes ./q2b_g

run2c:
	@echo ", Gather 8-1"
	mpirun -np 8 -npernode 1 -hostfile ../nodes ./q2c
	@echo ", Gather 8-2"
	mpirun -np 8 -npernode 2 -hostfile ../nodes ./q2c
	@echo ", Gather 8-4"
	mpirun -np 8 -npernode 4 -hostfile ../nodes ./q2c
	@echo ", Gather 8-8"
	mpirun -np 8 -npernode 8 -hostfile ../nodes ./q2c
