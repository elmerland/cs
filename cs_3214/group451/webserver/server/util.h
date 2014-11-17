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
#include <poll.h>

#include "list.h"

#define MAXLINE  256   /* max I/O line size */
#define MAXBUF   8192  /* max I/O buffer size */

// Stores the global settings of the local host.
struct glb_settings {
	// Port number for local host.
	char port[MAXLINE];
	// Host name of relay server.
	char r_host[MAXLINE];
	// ID of server for relay server.
	char server_id[MAXLINE];
	// Port number of relay server.
	char r_port[MAXLINE];
	// File path for root directory.
	char root[MAXLINE];
};

// Global variable that stores the settings for the web server.
struct glb_settings settings;

/* Persistent state for the robust I/O (Rio) package */
#define RIO_BUFSIZE 8192

// Robust I/O structure.
typedef struct {
    int rio_fd;                /* descriptor for this internal buf */
    int rio_cnt;               /* unread bytes in internal buf */
    char *rio_bufptr;          /* next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* internal buffer */
} rio_t;

/* Rio (Robust I/O) package */
ssize_t rio_readn(int fd, void * usrbuf, size_t n);
ssize_t rio_writen(int fd, void * usrbuf, size_t n);
void rio_readinitb(rio_t * rp, int fd);
ssize_t	rio_readnb(rio_t * rp, void *usrbuf, size_t n);
ssize_t	rio_readlineb(rio_t * rp, void *usrbuf, size_t maxlen);

// Number of method types.
#define METHOD_NUM 8

// Enumerators to store all types of HTTP methods.
enum method {
	OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT, M_UNK
};

// Enumerators to store the HTTP version.
enum version {
	ZERO, ONE
};

// Enumerators to store the connection request header status.
enum conn_type {
	CLOSE, KEEP_ALIVE
};

// Structure to store the arguments of a http request.
struct http_request {
	enum method req_method;		// Request line method.
	char uri[MAXBUF];			// Request line uri.
	enum version ver;			// Request line http version.

	enum conn_type connection;	// Connection request header.

	int	static_flag;			// Set to 1 if request is static.
	char arguments[MAXBUF];		// Arguments of URI.
};

struct mem_block {
	void * addr;
	struct list_elem elem;
};

// List that will hold pointers to all allocated blocks.
struct list block_list;

// Mutex lock for the block list.
pthread_mutex_t mem_lock;

/**
 * Sends a HTTP error code to the client.
 * This function was adapted form the CSAPP book.
 * URL: http://csapp.cs.cmu.edu/public/ics2/code/netp/tiny/tiny.c
 * @param fd       The socket descriptor for the client.
 * @param cause    The cause of the error.
 * @param errnum   The error number.
 * @param shortmsg A short description of the error.
 * @param longmsg  A long description of the error.
 * @param version The response version.
 */
void client_error(int fd, char *cause, char *errnum, 
	char *shortmsg, char *longmsg, int version);

/**
 * Sends a HTTP response to the client.
 * @param fd      The socket descriptor for the client.
 * @param title   The title of the response.
 * @param message The message of the response.
 * @param version The response version.
 */
void client_response(int fd, char * title, char * message, int version);

/**
 * Sends a message to the client.
 * @param fd      The socket descriptor for the client.
 * @param message The message to be sent to the client.
 * @param version The response version.
 */
void client_message(int fd, char * message, int version);

/**
 * Gets the enumerator that corresponds to the method of the request line.
 * @param  method The method of the request line.
 * @return        The enum that corresponds to the method. Return -1 if no match
 *                    was found.
 */
enum method get_method_enum(const char * method);

/**
 * Gets the URI arguments of the request line. It also determine weather the 
 * request is static or synthetic and.
 * @param uri     The URI of the request line.
 * @param request The http request structure where the results will be stored.
 */
void get_uri_arguments(const char * uri, struct http_request * request);

/**
 * Examines the URI arguments and tries to find a callback field name. If such
 * a field name is found the value of it is stored in the 'parameter' string and
 * 1 is returned. Otherwise a 0 is returned.
 * @param  arguments      The URI arguments.
 * @param  parameter      The string where the value of the field will be stored
 *                        if such a field exists.
 * @param  parameter_size The size of the parameter string.
 * @return                1 if a the field was found, 0 otherwise.
 */
int get_callback_value(const char * arguments, char * parameter, int parameter_size);

/**
 * Determines the MIME file type for the provided file name.
 * @param file_name      The file name.
 * @param file_type      Character array where the MIME file type wil be stored.
 * @param file_type_size The length of the 'file_type' character array.
 */
void get_file_type(const char * file_name, char * file_type, 
	int file_type_size);

/**
 * Determines if the file name is valid.
 * @param  file_name The file name.
 * @return           0 if the file name is valid, -1 if it is not valid.
 */
int validate_file_name(const char * file_name);

#endif
