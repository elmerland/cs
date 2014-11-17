
#include "util.h"

int main (int argc, char ** argv) {
	int clientfd, port;
	char * host, buf[MAXLINE];
	rio_t rio;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <host> <port> <input>\n", argv[0]);
		exit(0);
	}
	host = argv[1];
	port = atoi(argv[2]);

	// Get socket to server
	clientfd = open_clientfd(host, port);
	// Initialize robust I/O
	rio_readinitb(&rio, clientfd);

	// printf("Sending: %s", str);
	char * str = "GET /files/page.html HTTP/1.0\r\n\r\n";
	printf("Sending: %s", str);
	// Write to socket.
	rio_writen(clientfd, str, strlen(str));

	// Read from socket.
	while (rio_readlineb(&rio, buf, MAXLINE) > 0) {
		printf("%s", buf);
		// // Output result from socket to stdout
		// if (fputs(buf, stdout) <= 0) {
		// 	perror("Coudl not write to stdout");
		// }
	}

	printf("\n");

	close(clientfd);
	exit(0);
}
