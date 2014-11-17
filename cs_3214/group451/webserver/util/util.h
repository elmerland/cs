#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Misc constants */
#define	MAXLINE	 8192  /* max text line length */
#define MAXBUF   8192  /* max I/O buffer size */
#define LISTENQ  1024  /* second argument to listen() */

/* Persistent state for the robust I/O (Rio) package */
#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                /* descriptor for this internal buf */
    int rio_cnt;               /* unread bytes in internal buf */
    char *rio_bufptr;          /* next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* internal buffer */
} rio_t;

/* Client/server helper functions */
int open_clientfd(char *hostname, int portno);
int open_listenfd(int portno);

/* Rio (Robust I/O) package */
ssize_t rio_readn(int fd, void * usrbuf, size_t n);
ssize_t rio_writen(int fd, void * usrbuf, size_t n);
void rio_readinitb(rio_t * rp, int fd);
ssize_t	rio_readnb(rio_t * rp, void *usrbuf, size_t n);
ssize_t	rio_readlineb(rio_t * rp, void *usrbuf, size_t maxlen);

#endif