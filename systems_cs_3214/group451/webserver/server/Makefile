# Makefile for P4 - Web Server, CS 3214, Spring 2014
#
# Scott Sines
# Elmer Landaverde

CC = gcc
CFLAGS = -Wall -Werror
OBJECTS = http_request_handler.o list.o server.o http_response_handler.o \
		  threadpool.o util.o

# Build full project:

all: $(OBJECTS)
	$(CC) $(CFLAGS) -o sysstatd $(OBJECTS) -lpthread

tests:
	~cs3214/bin/server_unit_test.py -s $$PWD/sysstatd

# Rules for components:

server.o: server.c threadpool.o http_request_handler.o
	$(CC) $(CFLAGS) -c server.c

http_request_handler.o: http_request_handler.c http_request_handler.h util.o
	$(CC) $(CFLAGS) -c http_request_handler.c

http_response_handler.o: http_response_handler.c http_response_handler.h util.o
	$(CC) $(CFLAGS) -c http_response_handler.c

util.o: util.c util.h list.o global_tables.h
	$(CC) $(CFLAGS) -c util.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

threadpool.o: threadpool.c threadpool.h list.o
	$(CC) $(CFLAGS) -c threadpool.c

# Cleaning rule:

clean:
	rm -f *.o *.stackdump