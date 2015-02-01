#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define IN 0
#define OUT 1
#define ERR 2

int main(int argc, char ** argv) {
  int p1[2], p2[2];
  pipe(p1);
  pipe(p2);

  // Start first program
  if (fork() == 0) {
    char buff[80];
    sprintf(buff, "Starting: %s\n", argv[1]);
    write(ERR, buff, strlen(buff));

    // Overwrite standard out
    dup2(p1[OUT], OUT);
    close(p1[OUT]);
    close(p1[IN]);
    // Overwrite standard in
    dup2(p2[IN], IN);
    close(p2[OUT]);
    close(p2[IN]);

    char * cmds[] = {argv[1], (char *)0};
    execvp(argv[1], cmds);
  }

  // Start second program
  if (fork() == 0) {
    char buff[80];
    sprintf(buff, "Starting: %s\n", argv[2]);
    write(ERR, buff, strlen(buff));

    // Overwrite standard in
    dup2(p1[IN], IN);
    // Overwrite standard out
    dup2(p2[OUT], OUT);
    close(p1[OUT]);
    close(p2[OUT]);
    close(p1[IN]);
    close(p2[IN]);

    execvp(argv[2], &argv[2]);
  }

  close(p1[1]);
  close(p1[0]);
  close(p2[1]);
  close(p2[0]);

  wait(NULL);
  wait(NULL);
  return 0;
}
