#
# A simple Makefile to build 'watch'
#
#  to insure symbol are added for gdb to see		
CFLAGS= -g  -O0

OBJECTS= watch.o  esh-sys-utils.o
HEADERS= esh-sys-utils.h

default: watch sleep loop


# build the application 
watch:   esh-sys-utils.o watch.o 
	$(CC) $(CFLAGS) -o watch esh-sys-utils.o  watch.o   

sleep: sleep.c
	$(CC) $(CFLAGS) -o sleep sleep.c

loop: loop.c
	$(CC) $(CFLAGS) -o loop loop.c

esh-sys-utils.o: esh-sys-utils.c esh-sys-utils.h
	$(CC) $(CFLAGS) -c esh-sys-utils.c

main.o: watch.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(OBJECTS) watch loop sleep
