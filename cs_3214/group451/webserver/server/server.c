
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "threadpool.h"
// #include "util.h"
#include "http_request_handler.h"

// Number of threads for the thread pool to run.
#define THREAD_NUM 10

// The ID to pass to the relay server.
// #define RELAY_ID "elmer\r\n"

/**
 * This function connects to the relay host specified by settings.r_host and
 * starts listening for HTTP request.
 */
void run_relay_host(void);

/**
 * This function creates a socket for every IP address of this local host. It
 * then listens on all the sockets for incoming HTTP requests and responds
 * appropriately.
 */
void run_local_host(void);

/**
 * Starts polling all file descriptors associated with the socket address of
 * the local host. Whenever there is one file descriptor ready to be read from a
 * connection is established to the client and a new future is submitted to the
 * thread pool that will call the process_request function.
 * @param fds  Array of file descriptors to monitor.
 * @param nfds Number of file descriptors in the array.
 * @param tp   Pointer to a thread pool structure.
 */
void poll_fds(struct pollfd * fds, int nfds, struct thread_pool * tp);

/**
 * Get options from command line arguments and execute the appropriate
 * functions.
 * @param  argc Number of arguments.
 * @param  argv Argument values.
 * @return      This function never exits.
 */
int main (int argc, char *argv[])
{
  // Zero out the settings structure.
  bzero(&settings, sizeof(settings));
  // Get argument options
  int opt;
  int port_flag = 0, root_flag = 0, relay_flag = 0, id_flag = 0;

  opterr = 0;

  while ((opt = getopt(argc, argv, "p:r:R:i:")) != -1) {
    switch (opt) {
      case 'p': {
        strcpy(settings.port, optarg);
        port_flag = 1;
        break;
      }
      case 'R': {
        strcpy(settings.root, optarg);
        root_flag = 1;
        break;
      }
      case 'r': {
        char * tok = strtok(optarg, ":\0");
        strncpy(settings.r_host, tok, strlen(tok));
        tok = strtok(NULL, "\0");
        if (tok != NULL) {
          strncpy(settings.r_port, tok, strlen(tok));
        } else {
          printf("Missing relay host port number\n");
          abort();
        }
        relay_flag = 1;
        break;
      }
      case 'i':
      {
        strcpy(settings.server_id, optarg);
        id_flag = 1;
        break;
      }
      case '?': {
        printf("Missing option argument: -%c\n", optopt);
        return 1;
      }
      default:
        abort();
    }
  }
  if (port_flag == 0) {
    perror("Missing port number");
    exit(1);
  }
  if (root_flag == 0) {
    // Set root directory to default if it was not specified.
    strncpy(settings.root, "./", 3);
  }
  if(relay_flag && !id_flag)
  {
    strcpy(settings.server_id, "BDT\r\n");
  } else {
    sprintf(settings.server_id, "%s\r\n", settings.server_id);
  }

  // Initialize memory block list.
  list_init(&block_list);
  pthread_mutex_init(&mem_lock, NULL);

  // Ignore sigpipe signal
  signal (SIGPIPE, SIG_IGN);

  if (relay_flag) {
    // printf("Running server in relay mode...\n");
    run_relay_host();
  } else {
    // printf("Running server in local host mode...\n");
    run_local_host();
  }

  return 0;
}

void run_relay_host(void) {
  // Initialize address info structure.
  struct addrinfo * ai;
  struct addrinfo hints;

  // Initialize hints to zero.
  memset(&hints, '\0', sizeof(hints));
  hints.ai_flags = AI_ADDRCONFIG;
  hints.ai_socktype = SOCK_STREAM;

  // Get socket address for realy host.
  int e = getaddrinfo(settings.r_host, settings.r_port, &hints, &ai);
  if (e != 0) {
    perror("Could not get address info");
    exit(1);
  }

  // Create socket descriptor.
  int sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
  if (sock == -1) {
    perror("Could not get socket address");
    exit(1);
  }

  // Set socket option to reuse socket as soon as it is closed.
  int opt = 1;
  if (setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) < 0) {
    perror("Could not set re-use socket option");
    exit(1);
  }

  int result;
  #ifndef __APPLE__

  // Set keep alive options to socket.
  int keep_alive = 1;
  int idle = 120; // Send probe after 2 minutes of being idle.
  int count = 5;  // Number of probes before dropping the connection.
 

  result = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keep_alive, sizeof(int));
  if (result < 0) {
    perror("SO_KEEPALIVE");
    exit(1);
  }

  result = setsockopt(sock, SOL_TCP, TCP_KEEPIDLE, &idle, sizeof(int));
  if (result < 0) {
    perror("TCP_KEEPIDLE");
    exit(1);
  }

  result = setsockopt(sock, SOL_TCP, TCP_KEEPCNT, &count, sizeof(int));
  if (result < 0) {
    perror("TCP_KEEPCNT");
    exit(1);
  }

  #endif

  // Connect to socket.
  if (connect(sock, ai->ai_addr, ai->ai_addrlen) != 0) {
    perror("Could not connect to socket");
    exit(1);
  }

  // Free address info structure.
  freeaddrinfo(ai);
 
  // Create poll file descriptor structure.
  struct pollfd fds[1];
  fds[0].fd = sock;
  fds[0].events = POLLIN; // Tell poll to listen for read.

  // Send ID to relay host.
  if (write(fds[0].fd, settings.server_id, sizeof(settings.server_id)) == -1) {
    perror("Could not write to relay server");
    exit(1);
  }

  // Poll socket
  while ((result = poll(fds, 1, -1)) > 0) {
    short revent = fds[0].revents;
    // Determine if socket has error.
    if (revent & POLLHUP || revent & POLLERR || revent & POLLNVAL) {
      perror("Socket error");
      break;
    }
    if (revent & POLLIN) {
      // Socket is ready to be read from, process HTTP request.
      relay_client_handler(fds[0].fd);
    }
  }
  // Poll returned with error.
  if (result == -1) {
    perror("Could not poll");
  }
}

void run_local_host(void) {
  // Initialize address info structures.
  struct addrinfo *ai;
  struct addrinfo hints;
  // Zero out hints.
  memset (&hints, '\0', sizeof (hints));
  // Set hints to only take one IPv6 socket that maps the IPv4 into it.
  hints.ai_family = AF_INET6;
  hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_V4MAPPED;
  hints.ai_socktype = SOCK_STREAM;

  // Get socket address for this server at specified port
  int e = getaddrinfo (NULL, settings.port , &hints, &ai);
  if (e != 0)
  {
    perror("Could not get socket address");
  }

  // Get total number of socket addresses returned.
  int nfds = 0;
  struct addrinfo *runp = ai;
  while (runp != NULL)
  {
    ++nfds;
    runp = runp->ai_next;
  }

  // Create a poll file descriptor structure for each socket address.
  struct pollfd fds[nfds];

  // Iterate through all socket addresses and initialize a socket for each.
  for (nfds = 0, runp = ai; runp != NULL; runp = runp->ai_next) {

    // Try to get a socket descriptor.
    fds[nfds].fd = socket (runp->ai_family, runp->ai_socktype,
      runp->ai_protocol);
    if (fds[nfds].fd == -1)
    {
      perror("Could not get socket descriptor");
    }
    // Set the events that the poll file descriptor will listen for.
    fds[nfds].events = POLLIN;

    // Set socket option to be reused after close.
    int opt = 1;
    setsockopt (fds[nfds].fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt));

    // Bind socket to socket address.
    if (bind (fds[nfds].fd, runp->ai_addr, runp->ai_addrlen) != 0) {
      // Check for address already in use error.
      if (errno == EADDRINUSE) {
        perror("Could not bind socket address to socket");
      }
      // Close socket because it could not be binded.
      close (fds[nfds].fd);
      continue;
    }

    // Make socket a listening socket.
    if (listen (fds[nfds].fd, SOMAXCONN) != 0) {
      perror("Could not make socket a listening socket");
    }
    ++nfds;
  }

  // Free address info structure.
  freeaddrinfo (ai);

  // Initialize thread pool.
  struct thread_pool * tp = thread_pool_new(THREAD_NUM);

  // Start polling sockets.
  poll_fds(fds, nfds, tp);
}



void poll_fds(struct pollfd * fds, int nfds, struct thread_pool * tp) {

  // Infinitely loop to receive connections from sockets.
  while (1)
  {
    // Poll sockets until ready to read from. Set timeout to infinite.
    int n = poll (fds, nfds, -1);
    if (n == -1) {
      perror("Could not poll file descriptors");
    } else if (n == 0) {
      // No file descriptors had events.
      continue;
    }

    // Iterate through sockets to find the ones ready to be read from.
    int i;
    for (i = 0; i < nfds; ++i)
    {
      short revent = fds[i].revents;
      // Check for error.
      if (revent & POLLERR || revent & POLLNVAL || revent & POLLHUP) {
        perror("Socket has error");
        continue;
      }

      // Check if file descriptor is ready to be read from.
      if (!(revent & POLLIN)) {
        continue;
      }

      // Create socket address structure to store host information.
      struct sockaddr_storage rem;
      socklen_t remlen = sizeof (rem);
      printf("Accept new connection\n");
      // Accept connection from socket and get new file descriptor.
      int fd = accept (fds[i].fd, (struct sockaddr *) &rem, &remlen);
      if (fd == -1) {
        perror("Could not accept socket descriptor connection");
        continue;
      }

      // Add future to thread pool and handle HTTP request.
      thread_pool_submit(tp, local_client_handler, (void *)(intptr_t)fd);
    }
  }
}
