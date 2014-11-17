
#include "http_response_handler.h"

// Set time limit for runloop timer.
#define TIMER_SECONDS 15

// Block size when calling mmap (64 MB)
#define BLOCK_SIZE 67108864

/**
 * This function runs for TIMER_SECONDS seconds in a infinite while loop.
 * @param  args NULL
 * @return      NULL
 */
static void * timer(void * args);

/**
 * Formats the information in the /proc/loadavg file and formats it into a JSON
 * file before returning it to the client.
 * @param fd       The fd to write the response on.
 * @param callback The callback function for JSON file.
 * @param request  The http request structure.
 */
void serve_synthetic_loadavg(int fd, char * callback, 
	struct http_request * request);

/**
 * Formats the information in the /proc/meminfo file and formats it into a JSON
 * file before returning it to the client.
 * @param fd       The fd to write the response on.
 * @param callback The callback function for JSON file.
 * @param request  The http request structure.
 */
void serve_synthetic_meminfo(int fd, char * callback, 
	struct http_request * request);

/**
 * Allocates a memory block of size BLOCK_SIZE and brings it into main memory.
 * @param fd      The fd to write the response on.
 * @param request The http request structure.
 */
void serve_synthetic_allocanon(int fd, struct http_request * request);

/**
 * Frees the last memory block to be allocated by allocanon.
 * @param fd      The fd to write the response on.
 * @param request The http request structure.
 */
void serve_synthetic_freeanon(int fd, struct http_request * request);

/**
 * Forks a new process that executes a loop for TIMER_SECONDS seconds.
 * @param fd      The fd to write the response on.
 * @param request The http request structure.
 */
void serve_synthetic_runloop(int fd, struct http_request * request);

/**
 * Given the value of the callback function, this function sets the opneing and
 * closing string values for the JSON file.
 * @param args         The callback function or NULL if there is none.
 * @param opening      String where the opening string will be stored at.
 * @param opening_size The size of the opening string.
 * @param closing      String where the closing string will be stored at. This
 *                     string has to be at least of length 2.
 */
static void get_json_callback(char * args, char * opening, int opening_size, 
	char * closing);

/**
 * Create the HTTP response for a JSON file, where 'body' is the contents of the
 * JSON file.
 * @param fd   The socket descriptor.
 * @param body The body of the response.
 */
static void write_json_response(int fd, char * body, int version);

void serve_static_request(int fd, struct http_request * request) {
	// Check that the UIR argument is not empty.
	if (strlen(request->arguments) == 0) {
		client_error(fd, "No file name", "404", "File Not Found", 
			"Webserver could not find the requested file (1)", request->ver);
		return;
	}

	// Validate argument.
	if (validate_file_name(request->arguments) == -1) {
		client_error(fd, request->arguments, "404", "Invalid File Name", 
			"Webserver could not find the requested file (2)", request->ver);
		return;
	}

	// Create string for file name.
	char file_name[MAXLINE];
	file_name[0] = '\0';

	// Get root directory
	strncat(file_name, settings.root, MAXLINE);

	// Add file path to root directory.
	int len = strlen(settings.root) + 1;
	strncat(file_name, request->arguments, MAXLINE - len);

	// Get file information.
	struct stat file_stat;
	if (stat(file_name, &file_stat) < 0) {
		client_error(fd, file_name, "404", "File Not Found", 
			"Webserver could not find the requested file (3)", request->ver);
		return;
	}

	// Test if file is a regular file and if it has read permissions.
	if (!(S_ISREG(file_stat.st_mode)) || !(S_IRUSR & file_stat.st_mode)) {
        client_error(fd, file_name, "403", "Forbidden",
                    "Webserver couldn't read the file (1)", request->ver);
        return;
    }

    int filesize = file_stat.st_size;	// File size.
    int srcfd;	// File descriptor to target file.
    char * srcp;	// Pointer to memory block mapped to target file.
    char filetype[MAXLINE];	// File type MIME.
    char buf[MAXBUF];		// Buffer for the HTTP response line and headers.
 
    // Determine the file type MIME.
    get_file_type(file_name, filetype, MAXLINE);

    // Build the HTTP response.
    bzero(buf, MAXBUF);
    sprintf(buf, "HTTP/1.%c 200 OK\r\n", request->ver == ONE ? '1':'0');
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);

    // Open files descriptor to target file.
    if ((srcfd = open(file_name, O_RDONLY, 0)) < 0) {
    	client_error(fd, file_name, "403", "Forbidden",
                    "Webserver couldn't read the file (2)", request->ver);
    	return;
    }

    // Map file into main memory.
    srcp = mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    if (srcp == MAP_FAILED) {
    	client_error(fd, file_name, "403", "Forbidden",
                    "Webserver couldn't read the file (3)", request->ver);
    	return;
    }

    // Close files descriptor to target file.
    close(srcfd);

    // Write HTTP response line and headers to socket.
    rio_writen(fd, buf, strlen(buf));
    // Write the files contents to the socket.
    rio_writen(fd, srcp, filesize);

    // Free memory block allocated for file transfer.
    munmap(srcp, filesize);
}

void serve_synthetic_request(int fd, struct http_request * request) {
	// Get the URI path.
	char * path = request->arguments;;
	char * args = NULL;
	// Split the URI path from the arguments
	char * question_mark = strstr(request->arguments, "?");
	// Determine if the URI has arguments.
	if (question_mark != NULL) {
		*question_mark = '\0';
		args = question_mark + 1;
	}

	char * loadavg = "/loadavg";
	char * meminfo = "/meminfo";
	char * allocanon = "/allocanon";
	char * freeanon = "/freeanon";
	char * runloop = "/runloop";
	// Determine the synthetic function to be run.
	if (strncmp(path, loadavg, strlen(loadavg) + 1) == 0) {
		// Get the callback field from arguments.
		char callback[MAXLINE];
		if (!get_callback_value(args, callback, MAXLINE)) {
			serve_synthetic_loadavg(fd, NULL, request);
		} else {
			serve_synthetic_loadavg(fd, callback, request);
		}
		
	} else if (strncmp(path, meminfo, strlen(meminfo) + 1) == 0) {
		// Get the callback field from arguments.
		char callback[MAXLINE];
		if (!get_callback_value(args, callback, MAXLINE)) {
			serve_synthetic_meminfo(fd, NULL, request);
		} else {
			serve_synthetic_meminfo(fd, callback, request);
		}
	} else if (strncmp(path, allocanon, strlen(allocanon) + 1) == 0) {
		serve_synthetic_allocanon(fd, request);
	} else if (strncmp(path, freeanon, strlen(freeanon) + 1) == 0) {
		serve_synthetic_freeanon(fd, request);
	} else if (strncmp(path, runloop, strlen(runloop) + 1) == 0) {
		serve_synthetic_runloop(fd, request);
	} else {
		client_error(fd, request->arguments, "404", "File Not Found", 
			"Webserver could not find the requested synthetic function", 
			request->ver);
	}
}

void serve_synthetic_loadavg(int fd, char * callback, 
	struct http_request * request) {
	
	// Get file descriptor for the /proc/loadavg file
	int loadavg_fd = open("/proc/loadavg", O_RDONLY, 0);
	if (loadavg_fd == -1) {
		client_error(fd, "Loadavg", "500", "Internal server error",
			"Could not get load average (1)", request->ver);
		return;
	}

	// Read loadvg file.
	char buf[MAXLINE];
	rio_t rio;
	rio_readinitb(&rio, loadavg_fd);
	if (rio_readlineb(&rio, buf, MAXLINE) <= 0) {
		client_error(fd, "Loadavg", "500", "Internal server error",
			"Could not get load average (2)", request->ver);
		close(loadavg_fd);
		return;
	}
	// Close file descriptor for loadavg file.
	close(loadavg_fd);

	// Get the information from load average.
	char load1[MAXLINE];
	char load2[MAXLINE];
	char load3[MAXLINE];
	char c_threads[MAXLINE];
	char t_threads[MAXLINE];
	sscanf(buf, "%s %s %s %[^'/'']%*['/'']%s", load1, load2, load3, c_threads, 
		t_threads);

	// Get callback opening and closing tags.
	char opening[MAXLINE];
	char closing[2];
	get_json_callback(callback, opening, MAXLINE, closing);

	// Write HTTP response body.
	char body[MAXBUF];
	sprintf(body, "%s{\"total_threads\": \"%s\", "
		"\"loadavg\": [\"%s\", \"%s\", \"%s\"], \"running_threads\": \"%s\"}%s", 
		opening, t_threads, load1, load2, load3, c_threads, closing);

	// Write response to client.
	write_json_response(fd, body, request->ver);
}

void serve_synthetic_meminfo(int fd, char * callback, 
	struct http_request * request) {

	// Get file descriptor for the /proc/meminfo file.
	int meminfo_fd = open("/proc/meminfo", O_RDONLY, 0);
	if (meminfo_fd == -1) {
		client_error(fd, "Meminfo", "500", "Internal server error",
			"Could not get memory information (1)", request->ver);
		return;
	}

	// Get callback opening and closing tags.
	char opening[MAXLINE];
	char closing[2];
	get_json_callback(callback, opening, MAXLINE, closing);

	
	char result[MAXBUF];	// Buffer to store the JSON body.
	sprintf(result, "%s{", opening);	// Write opening tag to JSON string.

	rio_t rio;
	rio_readinitb(&rio, meminfo_fd);	// Initialize rio structure.
	int error;	// Holds rio read error.
	int add_comma = 0;	// Flag to add a coma before name-value pair.
	char buf[MAXLINE];	// Buffer to store line of meminfo
	char name[MAXLINE];
	char value[MAXLINE];

	// Read meminfo file line by line.
	while ((error = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
		if (error == -1) {
			client_error(fd, "Meminfo", "500", "Internal server error",
				"Could not get memory information (2)", request->ver);
			close(meminfo_fd);
			return;
		}

		// Get name a value from line in meminfo
		sscanf(buf, "%[^':']%*[':'] %s", name, value);

		// Add comma to separate name value pairs.
		if (add_comma) {
			sprintf(result, "%s, ", result);
		}

		// Add name-value pair to result.
		sprintf(result, "%s\"%s\": \"%s\"", result, name, value);
		add_comma = 1;
	}
	// Close meminfo file descriptor.
	close(meminfo_fd);
	
	// Add closing tag to JSON result.
	sprintf(result, "%s}%s", result, closing);

	// Write response to client.
	write_json_response(fd, result, request->ver);
}

void serve_synthetic_allocanon(int fd, struct http_request * request) {

	// Allocate a new memory block.
	void * addr = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (addr == MAP_FAILED) {
		client_error(fd, "Allocanon", "500", "Internal server error", 
			"Could not allocate block", request->ver);
		return;
	}

	// Zero out memory block to bring it into memory.
	bzero(addr, BLOCK_SIZE);

	// Allocate memory for a new mem_block.
	struct mem_block * b = malloc(sizeof(struct mem_block));
	// Set address of memory block.
	b->addr = addr;

	// Lock mutex to modify the block list.
	pthread_mutex_lock(&mem_lock);
	// Add mem_block to block list.
	list_push_front(&block_list, &b->elem);
	size_t size = list_size(&block_list);
	// Realize mutex lock.
	pthread_mutex_unlock(&mem_lock);

	// Build HTTP response message.
	char message[MAXLINE];
	sprintf(message, "Allocated blocks: <b>%d</b>", (int)size);

	// Send response to client.
	client_message(fd, message, request->ver);
}

void serve_synthetic_freeanon(int fd, struct http_request * request) {

	// Lock mutex to modify the block list.
	pthread_mutex_lock(&mem_lock);

	// Check if list is empty.
	if (!list_empty(&block_list)) {
		// Remove the front element of the list.
		struct list_elem * block_elem = list_pop_front(&block_list);
		// Get the block address.
		struct mem_block * block = list_entry(block_elem, struct mem_block, elem);
		// Free the memory block.
		if (munmap(block->addr, BLOCK_SIZE) == -1) {
			client_error(fd, "Freeanon", "500", "Internal server error",
				"Could not free memory block", request->ver);
			// Push memory block back into the list.
			list_push_front(&block_list, block_elem);
			// Unlock mutex.
			pthread_mutex_unlock(&mem_lock);
			return;
		}
		// Free memory mem_block structure.
		free(block);
	}
	// Get size of list.
	size_t size = list_size(&block_list);

	// Release mutex lock.
	pthread_mutex_unlock(&mem_lock);

	// Build HTTP response message.
	char message[MAXLINE];
	sprintf(message, "Allocated blocks: <b>%d</b>", (int)size);

	// Send message to client.
	client_message(fd, message, request->ver);
}

void serve_synthetic_runloop(int fd, struct http_request * request) {	

	// Fork a new child process.
	pid_t pid = fork();
	if (pid == -1) {
		client_error(fd, "Runloop", "500", "Internal server error",
			"Could not start run loop", request->ver);
		return;
	}

	if (pid == 0) {
		// In child process.
		pid = fork();	// Fork a new child process
		if (pid == 0) {
			// In grandchild process.
			timer(NULL);	// Run the timer function.
			exit(EXIT_SUCCESS);
		} else {
			// In child process, exit.
			exit(EXIT_SUCCESS);
		}
	} else {
		// Reap child process, leaving grand child as orphan. Orphan processes
		// will be reaped by the shell, so no need for the server to wait until
		// it terminates.
		wait(NULL);
		// Send response to client.
		client_message(fd, "<em>Loop process was executed!</em>", request->ver);
	}
}

static void * timer(void * args) {
	// Execute the loop
	time_t start = clock();
	// Infinite loop for TIMER_SECONDS seconds.
	while (1) {
		time_t now = clock();
		double dif = (now - start)/CLOCKS_PER_SEC;
		if (dif > TIMER_SECONDS) {
			break;
		}
	}
	return NULL;
}

static void get_json_callback(char * args, char * opening, int opening_size, 
	char * closing) {
	// Zero out the opening tag
	bzero(opening, opening_size);
	if (args == NULL) {
		// If the callback is null then the opening are closing tags are emtpy.
		opening[0] = '\0';
		closing[0] = '\0';
	} else {
		// Set the opening and closing tags to their appropriate values.
		strncpy(opening, args, MAXLINE - 2);
		opening[strlen(opening)] = '(';
		closing[0] = ')';
	}
}

static void write_json_response(int fd, char * body, int version) {
	char buf[MAXBUF];
	// Determine HTTP version
	if (version == ONE) {
		sprintf(buf, "HTTP/1.1 200 OK\r\n");
	} else {
		sprintf(buf, "HTTP/1.0 200 OK\r\n");	
	}
	// Build HTTP headers.
	sprintf(buf, "%sContent-type: application/json\r\n", buf);
	sprintf(buf, "%sContent-length: %d\r\n\r\n", buf, (int)strlen(body));

	// Write HTTP response line and headers.
	rio_writen(fd, buf, strlen(buf));
	// Write JSON file.
	rio_writen(fd, body, strlen(body));
}
