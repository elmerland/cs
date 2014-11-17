#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <stdlib.h>

#define OUT 1
#define IN 0

int main(int argc, char** argv) {

	int fd_down[2];
	int fd_up[2];

	pipe(fd_down);
	pipe(fd_up);

	if (fork() == 0) {
		printf("Starting: %s\n", argv[1]);

		// Pipe Write: This child writes to this pipe
		dup2(fd_down[OUT], OUT);
		close(fd_down[OUT]);
		close(fd_down[IN]);

		// Pipe Read: This child reads from this pipe
		dup2(fd_up[IN], IN);
		close(fd_up[IN]);
		close(fd_up[OUT]);

		// Execute first program
		char *cmd[] = {argv[1], (char *)0};
		execvp(argv[1], cmd);

		exit(0);
	}

	if (fork() == 0) {
		printf("Starting: %s\n", argv[2]);

		// Pipe Read: This child reads from this pipe
		dup2(fd_down[IN], IN);
		close(fd_down[IN]);
		close(fd_down[OUT]);

		// Pipe Write: This child writes to this pipe
		dup2(fd_up[OUT], OUT);
		close(fd_up[OUT]);
		close(fd_up[IN]);

		// Execute second program
		execvp(argv[2], &argv[2]);

		exit(0);
	}

	close(fd_up[IN]);
	close(fd_up[OUT]);
	close(fd_down[IN]);
	close(fd_down[OUT]);

	wait(NULL);
	wait(NULL);

	return 0;
}
