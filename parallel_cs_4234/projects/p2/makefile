.PHONY: all run compile 1 2

all: prob1 run1

1: prob1 run1

2: prob2 run2

compile: prob1 prob2

prob1: prob1.c
	gcc -Werror -Wall -pthread -o prob1 prob1.c

prob2: prob2.c
	gcc -Werror -Wall -pthread -o prob2 prob2.c

run1:
	sh prob1.sh 8 8 1048576

run2:
	./prob2 5 0 10 20 2
	./prob2 20 100 999 1000 15
	./prob2 20 100 999 10 15
	./prob2 10 100 999 10 15
	./prob2 10 100 999 20 15
