
#include <stdlib.h>

#include "http_request_handler.h"

#define TIMEOUT 30

/**
 * This function reads and processes an HTTP request.
 * @param  fd The file descriptor that connects to the client.
 * @return    -1 if the connection should be terminated, 0 if the connection
 *               should be kept alive.
 */
static int process_request(int fd);

/**
 * Extracts the parameters from the request line and sets the correct settings
 * in the http request structure.
 * @param  fd           The file descritor to output errors to.
 * @param  request_line The request line.
 * @param  request      The http request structure.
 * @return              -1 if the connection should be killed, -2 if the
 *                         connection should be kept alive.
 */
static int parse_request_line(int fd, const char * request_line, int request_line_size,
  struct http_request * request);

/**
 * Reads through all of the request headers and sets all appropriate settings
 * to the http request structure.
 * @param  rio     The rio structure to read the request headers from.
 * @param  request The http request structure.
 * @return         -1 if there was an error while reading form the socket.
 */
static int parse_request_headers(rio_t * rio, struct http_request * request);



/**
 * Determines the error code to be returned depending on what HTTP version the
 * request is in.
 * @param  ver The HTTP request version.
 * @return     -1 if the connection should be terminated, -2 if the connection
 *                should not be terminated.
 */
static int return_error(enum version ver);

/**
 * Gets the parameters for the HTTP request line.
 * @param  rl       The request line string
 * @param  rl_size  The size of the request line string
 * @param  m        The method string.
 * @param  m_size   The size of method.
 * @param  uri      The URI string.
 * @param  uri_size The size of URI.
 * @param  v        The version.
 * @param  v_size   The size of version.
 * @return          0 if all the paramters where obtained, -1 otherwise.
 */
static int get_request_line_parameters(const char * rl, int rl_size, char * m, int m_size, char * uri, int uri_size, char * v, int v_size);

void * local_client_handler(void * arg) {
  // Get file descriptor.
  int fd = (int)(intptr_t)arg;

  // Set timeout for socket
  struct timeval timeout;
  timeout.tv_sec = TIMEOUT * 1000;
  timeout.tv_usec = 0;

  setsockopt (fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  // Wrap file descriptor in pollfd
  struct pollfd fds[1];
  fds[0].fd = fd;
  fds[0].events = POLLIN;
  
  int ret;
  printf("Start handling client\n");
  while ((ret = poll(fds, 1, TIMEOUT)) > 0) {
    short revent = fds[0].revents;
    // Check if socket is still open.
    if (revent & POLLHUP || revent & POLLERR || revent & POLLNVAL) {
      // Check for socket error.
      // perror("Socket error (1)");
      break;
    }
    if (revent & POLLIN) {  // Check if data is ready to be read
      printf("Process new request\n");
      // Process HTTP request
      if (process_request(fd) == -1) {
        // If result equal to -1 close the connection.
        break;
      }
    } 
  }

  if (ret == -1) {
    perror("Could not poll socket\n");
  }

  printf("Close client connection\n");

  // Close socket.
  close(fd);
  return NULL;
}

void relay_client_handler(int fd) {
  // Wrap socket in poll structure.
  struct pollfd fds[1];
  fds[0].fd =fd;
  fds[0].events = POLLIN;

  // Poll for new HTTP request.
  int result;
  while ((result = poll(fds, 1, TIMEOUT * 1000)) > 0) {
    short revent = fds[0].revents;
    if (revent & POLLHUP || revent & POLLERR || revent & POLLNVAL) {
      perror("Socket error (2)");
      break;
    }
    if (revent & POLLIN) {
      // Process HTTP request
      if (process_request(fds[0].fd) == -1) {
        // If result equal to -1 exit out of this function.
        break;
      }
    }
  }
}

static int process_request(int fd) {

  // Setup buffer and rio.
  rio_t rio;
  rio_readinitb(&rio, fd);
  char buf[MAXBUF];

  // Read request line.
  int result = rio_readlineb(&rio, buf, MAXBUF);

  // Check for end of file.
  if (result == 0) {
    return -1;
  }
  // Check for read error.
  if (result < 0) {
    perror("Could not read from socket descriptor");
    return -1;
  }
  // Check for overflow
  if (buf[result - 1] != '\n') {
    client_error(fd, "Overflow", "500", "Internal server error",
      "HTTP request line is too long", ONE);
    return 0;
  }

  // Parse request line.
  struct http_request request;
  result = parse_request_line(fd, buf, MAXBUF, &request);
  if (result == -1) {
    client_error(fd, buf, "501", "Not Implemented",
      "HTTP request line is invalid (1)", ZERO);
    return -1;
  } else if (result == -2) {
    client_error(fd, buf, "501", "Not Implemented",
      "HTTP request line is invalid (2)", ONE);
    return 0;
  }

  result = parse_request_headers(&rio, &request);
  if (result == -1) {
    client_error(fd, buf, "500", "Internal server error",
      "Could not read request headers", request.ver);
    return -1;
  }

  // Serve content to client.
  if (request.static_flag) {
    serve_static_request(fd, &request);
  } else {
    serve_synthetic_request(fd, &request);
  }

  // Finish HTTP request.
  if (request.connection == CLOSE) {
    // Close connection.
    return -1;
  } else {
    // Keep connection alive.
    return 0;
  }
}

static int parse_request_line(int fd, const char * request_line, int request_line_size, 
  struct http_request * request) {
  
  // Initialize variables to hold request line parameters.
  char method[MAXLINE];
  char uri[MAXBUF];
  char version[MAXLINE];

  // Get request line parameters
  if (get_request_line_parameters(request_line, request_line_size, method, MAXLINE, uri, MAXBUF, version, MAXLINE) == -1) {
    return -1;
  }

  // Get request version
  if (strcasecmp(version, "HTTP/1.0") == 0) {
    request->ver = ZERO;
  } else if (strcasecmp(version, "HTTP/1.1") == 0) {
    request->ver = ONE;
  } else {
    return -1;
  }

  // Get request method
  request->req_method = get_method_enum(method);
  if (request->req_method == M_UNK) {
    // Request method is unknow.
    return return_error(request->ver);
  }
  if (request->req_method != GET) {
    // Request method is not supoprted.
    return return_error(request->ver);
  }

  // Get request UIR
  get_uri_arguments(uri, request);

  // Set default value for connection keep-alive value.
  if (request->ver == ONE) {
    request->connection = KEEP_ALIVE;
  } else {
    request->connection = CLOSE;
  }

  return 0;
}

static int parse_request_headers(rio_t * rio, struct http_request * request) {

  char buf[MAXLINE];  // Create buffer for request headers.
  int result = rio_readlineb(rio, buf, MAXLINE);  // Read first request header.
  while (result > 0) {
    // Check if line is empty.
    if (strcmp(buf, "\r\n") == 0) {
      return 0;
    }
    if (request->ver == ONE) {
      // Check if request header specifies connection
      if (strcasecmp(strtok(buf, ":"), "Connection") == 0) {
        char * tok = strtok(NULL, " \r\n");
        // Get the value of the conneciton header.
        if (strcasecmp(tok, "Keep-Alive") == 0) {
          request->connection = KEEP_ALIVE;
        } else if (strcasecmp(tok, "Close") == 0) {
          request->connection = CLOSE;
        }
      }
    }
    // Read the next line.
    result = rio_readlineb(rio, buf, MAXLINE);
  }

  // Return with error.
  if (result <= 0) {
    return -1;
  } else {
    return 0;
  }
}

static int return_error(enum version ver) {
  if (ver == ONE) {
    // Reurn error but keep connection alive.
    return -2;
  } else {
    // Return error and kill connection.
    return -1;
  }
}

static int get_request_line_parameters(const char * rl, int rl_size, char * m,
  int m_size, char * uri, int uri_size, char * v, int v_size) {

  // Make a copy of the request line
  char rl_cpy[rl_size];
  strncpy(rl_cpy, rl, rl_size);
  // Make sure that the request line has a null terminator
  rl_cpy[rl_size - 1] = '\0';

  // Tokenize the request line on space and get the request method.
  char * tok = strtok(rl_cpy, " \r\n");
  if (tok != NULL) {
    strncpy(m, tok, m_size);  // Copy method into string.
    m[m_size - 1] = '\0';
  } else {
    return -1;
  }

  // Tokenize again and get the request URI
  tok = strtok(NULL, " \r\n");
  if (tok != NULL) {
    strncpy(uri, tok, uri_size);  // Copy URI into string.
    uri[uri_size - 1] = '\0';
  } else {
    return -1;
  }

  // Tokenize again and get the request version.
  tok = strtok(NULL, " \r\n");
  if (tok != NULL) {
    strncpy(v, tok, v_size);  // Copy version into string.
    v[v_size - 1] = '\0';
  } else {
    return -1;
  }

  return 0;
 }
