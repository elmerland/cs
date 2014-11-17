
#include "util.h"

/******************************** 
 * Client/server helper functions
 ********************************/
/*
 * open_clientfd - open connection to server at <hostname, port> 
 *   and return a socket descriptor ready for reading and writing.
 */
int open_clientfd(char *hostname, int port) 
{
	int clientfd;
	struct hostent *hp;
	struct sockaddr_in serveraddr;

	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Could not get socket descriptor");
		return -1; /* Check errno for cause of error */
	}

	/* Fill in the server's IP address and port */
	if ((hp = gethostbyname(hostname)) == NULL) {
		perror("Could not get host name.");
		return -1; /* Check h_errno for cause of error */
	}

	bzero((char *) &serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	bcopy((char *)hp->h_addr_list[0], 
	  (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
	serveraddr.sin_port = htons(port);

	/* Establish a connection with the server */
	if (connect(clientfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
		perror("Could not connect sockect to server address");
		return -1;
	}

	return clientfd;
}

/*  
 * open_listenfd - open and return a listening socket on port
 */
int open_listenfd(int port) 
{
	int listenfd, optval=1;
	struct sockaddr_in serveraddr;
  
	/* Create a socket descriptor */
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Could not get socket descriptor");
		return -1;
	}
 
	/* Eliminates "Address already in use" error from bind */
	int result = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
		(const void *)&optval , sizeof(int));
	if (result < 0) {
		perror("Could not set socket option");
		return -1;
	}

	/* Listenfd will be an endpoint for all requests to port
	   on any IP address for this host */
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);

	if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
		perror("Could not bind socket to server address");
		return -1;
	}

	/* Make it a listening socket ready to accept connection requests */
	if (listen(listenfd, LISTENQ) < 0) {
		perror("Could not make listening socket");
		return -1;
	}

	return listenfd;
}

/*********************************************************************
 * The Rio package - robust I/O functions
 **********************************************************************/
/*
 * rio_readn - robustly read n bytes (unbuffered)
 */
ssize_t rio_readn(int fd, void *usrbuf, size_t n) 
{
	size_t nleft = n;
	ssize_t nread;
	char *bufp = usrbuf;

	while (nleft > 0) {
		if ((nread = read(fd, bufp, nleft)) < 0) {
			if (errno == EINTR) { /* Interrupted by sig handler return */
				nread = 0;      /* and call read() again */
			} else {
				return -1;      /* errno set by read() */ 
			}
		} else if (nread == 0){
			break;              /* EOF */
		}
		nleft -= nread;
		bufp += nread;
	}

	return (n - nleft);         /* return >= 0 */
}

/*
 * rio_writen - robustly write n bytes (unbuffered)
 */
ssize_t rio_writen(int fd, void *usrbuf, size_t n) 
{
	size_t nleft = n;
	ssize_t nwritten;
	char *bufp = usrbuf;

	while (nleft > 0) {
		if ((nwritten = write(fd, bufp, nleft)) <= 0) {
			if (errno == EINTR) { /* Interrupted by sig handler return */
				nwritten = 0;    /* and call write() again */
			} else {
				return -1;       /* errno set by write() */
			}
		}
		nleft -= nwritten;
		bufp += nwritten;
	}

	return n;
}


/* 
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
	int cnt;

	while (rp->rio_cnt <= 0) {  /* Refill if buf is empty */
		rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, 
				   sizeof(rp->rio_buf));
		if (rp->rio_cnt < 0) {
			if (errno != EINTR) { /* Interrupted by sig handler return */
				return -1;
			}
		} else if (rp->rio_cnt == 0) { /* EOF */
			return 0;
		} else {
			rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
		}
	}

	/* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
	cnt = n;          
	if (rp->rio_cnt < n) {
		cnt = rp->rio_cnt;
	}
	memcpy(usrbuf, rp->rio_bufptr, cnt);
	rp->rio_bufptr += cnt;
	rp->rio_cnt -= cnt;
	return cnt;
}

/*
 * rio_readinitb - Associate a descriptor with a read buffer and reset buffer
 */
void rio_readinitb(rio_t *rp, int fd) 
{
	rp->rio_fd = fd;  
	rp->rio_cnt = 0;  
	rp->rio_bufptr = rp->rio_buf;
}

/*
 * rio_readnb - Robustly read n bytes (buffered)
 */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
{
	size_t nleft = n;
	ssize_t nread;
	char *bufp = usrbuf;
	
	while (nleft > 0) {
		if ((nread = rio_read(rp, bufp, nleft)) < 0) {
			return -1;          /* errno set by read() */ 
		} else if (nread == 0) {
			break;              /* EOF */
		}
		nleft -= nread;
		bufp += nread;
	}

	return (n - nleft);         /* return >= 0 */
}

/* 
 * rio_readlineb - robustly read a text line (buffered)
 */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
	int n, rc;
	char c, *bufp = usrbuf;

	for (n = 1; n < maxlen; n++) { 
		if ((rc = rio_read(rp, &c, 1)) == 1) {
			*bufp++ = c;
			if (c == '\n') {
				n++;
				break;
			}
		} else if (rc == 0) {
			if (n == 1) {
				return 0; /* EOF, no data read */
			} else {
				break;    /* EOF, some data was read */
			}
		} else {
			return -1;	  /* Error */
		}
	}

	*bufp = 0;
	return n-1;
}
