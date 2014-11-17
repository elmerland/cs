#ifndef HTTP_RESPONSE_HANDLER_H
#define HTTP_RESPONSE_HANDLER_H

#include <time.h>

#include "util.h"

/**
 * Serves a static file back to the client.
 * @param  fd      The file descriptor to the client.
 * @param  request The HTTP request structure.
 */
void serve_static_request(int fd, struct http_request * request);

/**
 * Serves a synthetic response back to the client.
 * @param  fd      The file descriptor to the client.
 * @param  request The HTTP request structure.
 */
void serve_synthetic_request(int fd, struct http_request * request);

#endif