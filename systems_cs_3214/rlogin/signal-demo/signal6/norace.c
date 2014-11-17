#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "esh-sys-utils.h"
#include "list.h"

/*
 * This program demonstrates that calling 'printf'
 * (or puts, for that matter, which is what gcc 
 * quietly changes the call to).
 * is not safe from within a signal handler that's
 * invoked asynchronously (like SIGCHLD's is)
 * 
 * Start the program, and you'll see it "get stuck"
 * after a while.  Attach gdb to it (gdb ./signaldeadlock)
 * "attach <pid>" in gdb, where pid is the process id.
 *
 * @author Godmar Back <godmar@gmail.com>
 * Written for CS 3214 Fall 2009, Virginia Tech
 */
static void
catch_child(int signo, siginfo_t* info, void* context)
{
    /* this call may reenter printf/puts! Bad! */
//    printf("Child exited!\n");
    /* reap all children */
    int status;
    int  pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0){
      /* if child terminated */
      if( WIFEXITED(status)) {
         /* remove child pid from list */
          remove_child(pid);
      }
    }
}



int
main()
{
    esh_signal_sethandler(SIGCHLD, catch_child);

    int i;
    for (i = 0; i < 100000; i++) {
        int pid = fork();
        if (pid == 0)
            exit(0);

        /* the child is running, it may exit
         * any time.... SIGCHLD may arrive
         * while adding or removing elements from the list
         */

       /* add child to list of active processes */
      esh_signal_block(SIGCHLD);
      add_child(pid);
      esh_signal_unblock(SIGCHLD);
      
    }
    printf("Terminating Successfully. \n");
    return 0;
}
