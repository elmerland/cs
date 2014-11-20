.PHONY: all compile

all: compile

compile: mptest.c prob1.c list.c list.h
	gcc -fopenmp -Wall -Werror -o mptest mptest.c
	gcc -fopenmp -Wall -Werror -o prob1 prob1.c
	gcc -pthread -Wall -Werror -o prob1_linear prob1_linear.c
	gcc -fopenmp -Wall -Werror -o prob2 prob2.c list.c
	gcc -Wall -Werror -o prob2_linear prob2_linear.c
	gcc -fopenmp -Wall -Werror -o prob3 prob3.c
	gcc -fopenmp -Wall -Werror -o prob3_linear prob3_linear.c

run1:
	./prob1 10 0 100 500 2
	./prob1_linear 10 0 100 500 1

run2:
	./prob2 3 textfiles/* | wc
	./prob2_linear textfiles/* | wc

runt:
	@echo "10mb memory" >> output.txt
	@echo "1 kb" >> output.txt
	./prob3 16 data_files/01* >> output.txt

	@echo "10 kb" >> output.txt
	./prob3 16 data_files/02* >> output.txt

	@echo "100 kb" >> output.txt
	./prob3 16 data_files/03* >> output.txt

	@echo "1 mb" >> output.txt
	./prob3 16 data_files/04* >> output.txt

	@echo "10 mb" >> output.txt
	./prob3 16 data_files/05* >> output.txt

	@echo "100 mb" >> output.txt
	./prob3 16 data_files/06* >> output.txt

	@echo "1 gb" >> output.txt
	./prob3 16 data_files/07* >> output.txt

	@echo "5 gb" >> output.txt
	./prob3 16 data_files/08* >> output.txt

run:
	./prob3 5 textfiles/text_small.txt > output.txt
	./prob3_linear textfiles/text_small.txt > output_linear.txt
	diff output.txt output_linear.txt
