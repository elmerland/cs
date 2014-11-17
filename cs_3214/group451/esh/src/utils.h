#ifndef UTILS
#define UTILS

#include <stdbool.h>

/**
 * If the SIGCHLD signal has being previously blocked it does nothing,
 * otherwise it blocks the SIGCHLD signal.
 * @param prev_blocked True if the SIGCHLG signal has being previously blocked.
 */
void block_sigchld(bool prev_blocked);

/**
 * If the SIGCHLD signal has being previously blocked it does nothing,
 * otherwise it unblocks the SIGCHLD signal.
 * @param prev_blocked True if the SIGCHLG signal has being previously blocked.
 */
void unblock_sigchld(bool prev_blocked);

#endif
