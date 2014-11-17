
OBJS= prime.o rngs.o bounded-buf.o int-pipe1.o int-pipe2.o int-pipe3.o int-pipe4.o \
      thread-pipe.o thread-ipipe.o  
EXECS= tp tip1 tip2 tip3 tip4
CFLAGS= -g0
LIBS = -lpthread -lm

default: tp tip1 tip2  tip3 tip4


tp: thread-pipe.o prime.o rngs.o
	$(CC) -o tp thread-pipe.o prime.o rngs.o $(LIBS)

tip1: thread-ipipe.o int-pipe1.o bounded-buf.o prime.o rngs.o
	$(CC) -o tip1 thread-ipipe.o int-pipe1.o bounded-buf.o prime.o rngs.o $(LIBS)

tip2: thread-ipipe.o int-pipe2.o bounded-buf.o prime.o rngs.o
	$(CC) -o tip2 thread-ipipe.o int-pipe2.o bounded-buf.o prime.o rngs.o $(LIBS)

tip3: thread-ipipe.o int-pipe3.o bounded-buf.o prime.o rngs.o
	$(CC) -o tip3 thread-ipipe.o int-pipe3.o bounded-buf.o prime.o rngs.o $(LIBS)

tip4: thread-ipipe.o int-pipe4.o bounded-buf.o prime.o rngs.o
	$(CC) -o tip4 thread-ipipe.o int-pipe4.o bounded-buf.o prime.o rngs.o $(LIBS)

thread-pipe.o: thread-pipe.c
	$(CC) -c thread-pipe.c

thread-ipipe.o: thread-ipipe.c 
	$(CC) -c thread-ipipe.c

bounded-buf.o: bounded-buf.c bounded-buf.h
	$(CC) -c bounded-buf.c

int_pipe1.o: int-pipe1.c int-pipe1.h bounded-buf.c bounded-buf.h
	$(CC) -c int-pipe1.c

int_pipe2.o: int-pipe2.c int-pipe2.h bounded-buf.c bounded-buf.h
	$(CC) -c int-pipe2.c

int_pipe3.o: int-pipe3.c int-pipe3.h bounded-buf.c bounded-buf.h
	$(CC) -c int-pipe3.c

int_pipe4.o: int-pipe4.c int-pipe4.h bounded-buf.c bounded-buf.h
	$(CC) -c int-pipe4.c

prime.o: prime.c prime.h
	$(CC) -c prime.c

rngs.o: rngs.c rngs.h
	$(CC) -c rngs.c

clean:
	rm -f $(OBJS) $(EXECS)
