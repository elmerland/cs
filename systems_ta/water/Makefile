
.PHONY: compile run

compile: water.c
	gcc -Wall -Werror -pthread -o water water.c
	gcc -Wall -Werror -pthread -o watercond watercond.c

run:
	./water

runc:
	./watercond

runv:
	valgrind --tool=helgrind ./water

runcv:
	valgrind --tool=helgrind ./watercond
