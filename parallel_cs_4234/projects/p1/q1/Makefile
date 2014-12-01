.PHONY: all compile run

all: compile run

compile: q1a_b q1a_sr q1b

run: run1a_b run1a_sr

# Compile file for quetion 1a - broadcast
q1a_b: q1a_b.c
	mpicc -o q1a_b q1a_b.c

# Compile file for quetion 1a - send/receive
q1a_sr: q1a_sr.c
	mpicc -o q1a_sr q1a_sr.c

# Compile file for quetion 1b
q1b: q1b.c
	mpicc -o q1b q1b.c

run1a_b:
	@echo ", Broadcast 4"
	mpirun -np 4 -map-by ppr:1:node -hostfile ../nodes ./q1a_b
	@echo ", Broadcast 8"
	mpirun -np 8 -map-by ppr:1:node -hostfile ../nodes ./q1a_b
	@echo ", Broadcast  16"
	mpirun -np 16 -map-by ppr:1:node -hostfile ../nodes ./q1a_b

run1a_sr:
	@echo ", SendRecv  4"
	mpirun -np 4 -map-by ppr:1:node -hostfile ../nodes ./q1a_sr
	@echo ", SendRecv  8"
	mpirun -np 8 -map-by ppr:1:node -hostfile ../nodes ./q1a_sr
	@echo ", SendRecv  16"
	mpirun -np 16 -map-by ppr:1:node -hostfile ../nodes ./q1a_sr

run1b:
	@echo ", inter-node,"
	mpirun -np 4 -map-by ppr:1:node -hostfile ../nodes ./q1b
	@echo ", intra-node,"
	mpirun -np 4 -map-by ppr:4:node -hostfile ../nodes ./q1b
