
#include "util.h"
#include "global_tables.h"

/**
 * Determine if the provided string 'str' ends with the provided suffix 
 * 'suffix'.
 * @param  str    The string to compare.
 * @param  suffix The suffix to match.
 * @return        1 if the string ends with the given suffix, 0 otherwise.
 */
static int ends_with(const char *str, const char *suffix);

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
	if ((size_t)rp->rio_cnt < n) {
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

	for (n = 1; (size_t)n < maxlen; n++) { 
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

/*********************************************************************
 * HTTP Wizard Helper Functions
 **********************************************************************/

void client_error(int fd, char *cause, char *errnum, 
	char *shortmsg, char *longmsg, int version) {
	
	char buf[MAXBUF], body[MAXBUF];

	/* Build the HTTP response body */
	sprintf(body, "<!DOCTYPE html><html><head><style>h1, h2, p {text-align:");
	sprintf(body, "%s center;}h1 {font-family: \"Helvetica Neue\", sans", body);
	sprintf(body, "%s-serif;font-size: 44pt;}h2 {font-family: Georgia, ", body);
	sprintf(body, "%sserif; font-size: 32pt;}h2 b {font-size: 36pt;}p {", body);
	sprintf(body, "%sfont-family: Georgia, serif; font-size: 24pt;}img ", body);
	sprintf(body, "%s{display: block; width: 400px; margin: 0 auto;}</s", body);
	sprintf(body, "%style></head><body><article><h1>The BDT Server</h1>", body);
	sprintf(body, "%s<hr><img src=\"http://cdn.teckler.com/images/OsSim", body);
	sprintf(body, "%spsons/d9e03e65f5195da85be986f022893a22.jpg\">", body);
	sprintf(body, "%s<h2><b>%s</b> - %s</h2>", body, errnum, shortmsg);
	sprintf(body, "%s<p>%s: <i>%s</i></p>", body, longmsg, cause);
	sprintf(body, "%s</article></body></html>\r\n", body);

	/* Print the HTTP response */
	if (version == ONE) {
		sprintf(buf, "HTTP/1.1 %s %s\r\n", errnum, shortmsg);
	} else {
		sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
	}
	sprintf(buf, "%sContent-type: text/html\r\n", buf);
	sprintf(buf, "%sContent-length: %d\r\n\r\n", buf, (int)strlen(body));

	rio_writen(fd, buf, strlen(buf));
	rio_writen(fd, body, strlen(body));
}

void client_response(int fd, char * title, char * message, int version) {
	// Create response for client.
	char buf[MAXLINE * 4], body[MAXLINE * 4];

	/* Build the HTTP response body */	
	sprintf(body, "%s<body>\r\n", body);
	sprintf(body, "%s%s\r\n", body, message);
	sprintf(body, "%s<hr><em>The BDT Web server</em>\r\n", body);

	/* Build the HTTP response */
	char * code = "200";
	char * code_desc = "OK";
	if (version == ONE) {
		sprintf(buf, "HTTP/1.1 %s %s\r\n", code, code_desc);
	} else {
		sprintf(buf, "HTTP/1.0 %s %s\r\n", code, code_desc);
	}
	sprintf(buf, "%sContent-type: text/html\r\n", buf);
	sprintf(buf, "%sContent-length: %d\r\n\r\n", buf, (int)strlen(body));

	rio_writen(fd, buf, strlen(buf));
	rio_writen(fd, body, strlen(body));
}

void client_message(int fd, char * message, int version) {
	char buf[MAXLINE * 4];

	/* Print the HTTP response */
	if (version == ONE) {
		sprintf(buf, "HTTP/1.1 %s %s\r\n", "200", "OK");
	} else {
		sprintf(buf, "HTTP/1.0 %s %s\r\n", "200", "OK");
	}
	sprintf(buf, "%sContent-type: text/html\r\n", buf);
	sprintf(buf, "%sContent-Length: %d\r\n\r\n", buf, (int)strlen(message));

	rio_writen(fd, buf, strlen(buf));
	rio_writen(fd, message, strlen(message));
}

enum method get_method_enum(const char * method) {
	int i;
	for (i = 0; i < METHOD_NUM; i++) {
		if (strcasecmp(method_table[i].m_str, method) == 0) {
			return method_table[i].m_enum;
		}
	}
	return M_UNK;
}

void get_uri_arguments(const char * uri, struct http_request * request) {
	// Set uri in request structure.
	strncpy(request->uri, uri, strlen(uri) + 1);

	// Determine if is a synthetic or static request.
	char * files = "/files/";
	const char * args;
	if (strncmp(uri, files, strlen(files)) == 0) {
		request->static_flag = 1;
		args = uri + strlen(files);
	} else {
		request->static_flag = 0;
		args = uri;
	}
	// Copy URI arguments into request structure.
	strncpy(request->arguments, args, strlen(args) + 1);
}

int get_callback_value(const char * arguments, char * parameter, int parameter_size) {
	// Check that the argument is not emtpy.
	if (arguments == NULL || strlen(arguments) == 0) {
		return 0;
	}
	// Copy the uri arguments
	char arg_cpy[MAXBUF];
	strncpy(arg_cpy, arguments, MAXBUF);

	char * field_name = "callback=";	// Field name to find.
	// Tokenize until the field name is found or the string ends.
	char * tok = strtok(arg_cpy, "&\0");
	while (tok != NULL) {
		if (strncmp(tok, field_name, strlen(field_name)) == 0) {
			// Get the field value.
			char * value = tok + strlen(field_name);
			int value_size = strlen(value) + 1;

			// Validate the callback function name.
			int i;
			for (i = 0; i < value_size; i++) {
				if (!isalnum(value[i]) && !value[i] == '_' && !value[i] == '.') {
					return 0;
				}
			}
			// Copy the field value to the parameter string.
			strncpy(parameter, value, parameter_size < value_size ? parameter_size : value_size);
			return 1;
		}
		tok = strtok(NULL, "&\0");
	}
	return 0;
}

void get_file_type(const char * file_name, char * file_type, int file_type_size) {

	// Loop through every element of the MIME type list.
	int i;
	for (i = 0; i < MIME_NUM; i++) {
		if (ends_with(file_name, mime_table[i].extension)) {
			strncpy(file_type, mime_table[i].type, file_type_size);
			return;
		}
	}
	// If no match was found return the defailt text/plain
	strncpy(file_type, default_type, file_type_size);
}

static int ends_with(const char *str, const char *suffix) {
    if (!str || !suffix) {
        return 0;
    }
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr) {
        return 0;
    }
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

int validate_file_name(const char * file_name) {
	// Make a copy of the file name.
	char file[MAXBUF];
	strncpy(file, file_name, MAXBUF);

	// Count tokens.
	int count = 0;
	char * token = strtok(file, "/");
	while (token != NULL) {
		// Check if token is a double dot.
		if (strcmp(token, "..") == 0) {
			count--;
		} else {
			count++;
		}

		// Check that the dot count is positive.
		if (count < 0) {
			return -1;
		}
		token = strtok(NULL, "/");
	}

	// Return valid file name.
	return 0;
}
