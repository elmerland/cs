.PHONY: all run compile strace

all: mydpipe run

compile: mydpipe.c
	gcc -static -Wall -Werror -O -o mydpipe mydpipe.c

run:
	./mydpipe wc gnetcat -l 1691

connect:
	./gnetcat sassafras 1691 < /etc/passwd

strace:
	rm my_output/*
	strace -f -ff -o my_output/my_output ./mydpipe wc gnetcat -l 1691
