#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "esh-sys-utils.h"

/*
 * This program monitors the change in state of child
 * processes that it creates. 
 *
 * @author Godmar Back <godmar@gmail.com>
 * Written for CS 3214 Fall 2009, Virginia Tech
 *
 * Modified by Dennis kafura in Spring 2014
 */

int done=0;

static void
catch_child(int signo, siginfo_t* info, void* context)
{
    /* reap all children and/or report status change */
    int status;
    int  pid;
    while ((pid = waitpid(-1, &status, WNOHANG|WCONTINUED|WUNTRACED)) > 0){
      if( WIFEXITED(status)) {
          printf("Received sigal that child process (%d) terminated. \n", pid);
          done++;
      }
      if (WIFSTOPPED(status)) { 
          printf("Received signal that child process (%d) stopped. \n", pid);
      }
      if (WIFCONTINUED(status)) {
          printf("Received signal that child process (%d) continued. \n", pid);
      }
      if( WIFSIGNALED(status)) {
          printf("Received signal that child process (%d) received signal [%d] \n", 
                  pid, WTERMSIG(status));
          done++;
      }
    }
}



int
main()
{
    esh_signal_sethandler(SIGCHLD, catch_child);

    int pid;
    /* create child process that sleeps before exiting normally */
    pid = fork();
    if (pid == 0) {
        execlp("./sleep", "sleep", NULL); 
        printf("Sleep child failed to launch. \n");
        exit(0);
    }
    else printf("Sleep process pid is %d \n", pid); 

    /* create child process that loops indefinitely */
    pid=fork();
    if(pid==0){ 
        execlp("./loop", "loop", NULL);
        printf("Loop child failed to launch. \n");
    }
    else printf("Loop process pid is %d \n", pid);
   
    while(done<2); /* wait for both children to terminate */

    printf("Watch Terminating Successfully. \n");
    return 0;
}
