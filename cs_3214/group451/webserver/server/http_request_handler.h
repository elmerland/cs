#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include "util.h"
#include "http_response_handler.h"

/**
 * Handles one connection with a client.
 * @param  arg The file descriptor that is linked to the client.
 * @return     NULL.
 */
void * local_client_handler(void * arg);

/**
 * Handles one HTTp/1.1 connection with a client.
 * @param fd The file descriptor that connects to the client.
 */
void relay_client_handler(int fd);

#endif
