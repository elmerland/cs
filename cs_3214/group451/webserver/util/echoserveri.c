
#include "util.h"

void echo(int connfd);

int main (int argc, char ** argv) {
	int listenfd, connfd, port;
	socklen_t clientlen;
	struct sockaddr_in clientaddr;
	struct hostent * hp;
	char * haddrp;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		exit(0);
	}

	port = atoi(argv[1]);

	listenfd = open_listenfd(port);

	while (1) {
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);

		// Determine the domain name and IP address of the client.
		hp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
			sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		haddrp = inet_ntoa(clientaddr.sin_addr);
		printf("Server connected to %s (%s)\n", hp->h_name, haddrp);

		echo(connfd);
		close(connfd);
	}
	exit(0);
}

void echo(int connfd) {
	size_t n;
	char buf[MAXLINE];
	rio_t rio;

	rio_readinitb(&rio, connfd);
	while ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
		printf("Server received: %zu bytes\n", n);
		rio_writen(connfd, buf, n);
	}
}
