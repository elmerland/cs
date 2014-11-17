#
# A simple Makefile to build 'race'
#
#  to insure symbol are added for gdb to see		
CFLAGS= -g  -O0

OBJECTS=print-pid.o list.o race.o norace.o esh-sys-utils.o
HEADERS=print-pid.h list.h esh-sys-utils.h

default: race 


# build the application 
race:   print-pid.o esh-sys-utils.o list.o race.o 
	$(CC) $(CFLAGS) -o race  print-pid.o esh-sys-utils.o  list.o  race.o  

norace: print-pid.o esh-sys-utils.o list.o norace.o
	$(CC) $(CFLAGS) -o norace print-pid.o esh-sys-utils.o list.o norace.o

esh-sys-utils.o: esh-sys-utils.c esh-sys-utils.h
	$(CC) $(CFLAGS) -c esh-sys-utils.c

list.o : list.c list.h
	$(CC) $(CFLAGS) -c list.c

print-pid.o: print-pid.h print-pid.c
	$(CC) $(CFLAGS) -c print-pid.c

race.o: race.c
	$(CC) $(CFLAGS) -c race.c

norace.o: norace.c
	$(CC) $(CFLAGS) -c norace.c

clean:
	rm -f $(OBJECTS) race norace
