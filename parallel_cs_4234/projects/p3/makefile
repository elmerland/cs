.PHONY: all compile

all: compile run

compile: mptest.c prob1.c list.c list.h
	gcc -fopenmp -Wall -Werror -o mptest mptest.c
	gcc -fopenmp -Wall -Werror -o prob1 prob1.c
	gcc -fopenmp -Wall -Werror -o prob2 prob2.c list.c
	gcc -Wall -Werror -o prob2_linear prob2_linear.c

run:
	./prob2 3 textfiles/* | wc
	./prob2_linear textfiles/* | wc

runf:
	./prob2 3 textfiles/* > output.txt
	./prob2_linear textfiles/* > output_linear.txt