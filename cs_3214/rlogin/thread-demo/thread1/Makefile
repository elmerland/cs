
OBJS= proc.o thread1.o thread2.o thread3.o
EXECS= proc thread1 thread2 thread3
CFLAGS= -g0

default: proc thread1 thread2 thread3


proc: proc.o
	$(CC) -o proc proc.o

proc.o: proc.c
	$(CC) -c proc.c

thread1: thread1.o
	$(CC) -o thread1 thread1.o -lpthread

thread1.o: thread1.c
	$(CC) -c thread1.c

thread2: thread2.o
	$(CC) $(CFLAGS) -o thread2 thread2.o -lpthread

thread2.o: thread2.c
	$(CC) $(CFLAGS) -c thread2.c

thread3: thread3.o
	$(CC) $(CFLAGS) -o thread3 thread3.o -lpthread

thread3.o: thread3.c
	$(CC) $(CFLAGS) -c thread3.c

clean:
	rm -f $(OBJS) $(EXECS)
