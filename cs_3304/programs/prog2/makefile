.PHONY: parser run tar

parser: parser.c
	gcc -static -Wall -Werror -o parser parser.c

run:
	./parser < data.txt

tar: parser
	tar -cvf landaverdeea03.tar README.txt parser.c parser data.txt output.txt makefile
