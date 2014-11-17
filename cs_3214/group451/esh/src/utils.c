
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "esh-sys-utils.h"
#include "utils.h"


void block_sigchld(bool prev_blocked) {
	if (!prev_blocked) {
		// Block SIGCHLD signal
		esh_signal_block(SIGCHLD);
		if (!esh_signal_is_blocked(SIGCHLD)) {
			perror("SIGCHLD could not be blocked");
			exit(1);
		}
	}
}

void unblock_sigchld(bool prev_blocked) {
	if (!prev_blocked) {
		// Unblock SIGCHLD signal
		esh_signal_unblock(SIGCHLD);
		if (esh_signal_is_blocked(SIGCHLD)) {
			perror("SIGCHLD could not be unblocked");
			exit(1);
		}
	}
}
