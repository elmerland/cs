/**
 * Plugin in that takes an input string displays it on the 
 * screen in a snake-like pattern then uses a random number
 * generator to calculate and display a fun message upon
 * successful completion of execution. Ctrl-Z and ctrl-C are
 * reset so that they end the plugin gracefully. These controls
 * are then set back to their initial state before the plugin
 * exits.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "../esh.h"
#include "../esh-sys-utils.h"

/* Maximum number of space characters that will be added to the input string */
#define WIDTH 60

/* Maximum number of lines that will be displayed */
#define LIMIT 1000

/* Maximum number of characters that can be stored in our string buffer */
#define BUFF_SIZE 256

/* Amount of time between each printed line (in microseconds) */
#define DELAY 10000

/* Variable to store whether the user wishes to end the plugins execution */
bool stop;

/* String buffer used for re-entrant printing to the terminal */
char str_buff[BUFF_SIZE];

/**
 * Signal handler used to handle a SIGTSTP (ctrl-Z) and SIGINT (ctrl-C). When
 * the user sends either of these signals, the plugin should gracefully
 * terminate. Signals are reset to their initial handlers upon prior to exit.
 */
static void yolo_handle(int signo);

/**
 * Initialize the plugin.
 */
static bool 
init_plugin(struct esh_shell *shell)
{
    printf("Plugin 'yolo' initialized...\n");
    return true;
}

/**
 * Run the 'yolo' command.
 */
static bool
yolo_run(struct esh_command * cmd)
{
    /* Check that the user is attempting to run the 'yolo' command */
    if (strcmp(cmd->argv[0], "yolo") != 0)
	{
        return false;
    }

    /* Initialize the stop boolean to false. Will be flipped upon user signal */
    stop = false;

    struct sigaction sa_sigint, sa_sigtstp;
    __sighandler_t old_sigint, old_sigtstp;

    /* Store old signal handler */
    sigaction(SIGINT, NULL, &sa_sigint);
    sigaction(SIGTSTP, NULL, &sa_sigtstp);
    old_sigint = sa_sigint.sa_handler;
    old_sigtstp = sa_sigtstp.sa_handler;

    /* Set new signal handler*/
    sa_sigint.sa_handler = yolo_handle;
    sa_sigtstp.sa_handler = yolo_handle;
    sigaction(SIGINT, &sa_sigint, NULL);
    sigaction(SIGTSTP, &sa_sigtstp, NULL);

    /*
     * Determine the input string. If the user did not enter a string, uses 
     * 'OO' as the default string.
     */
    char* input_string;

    if(cmd->argv[1] != NULL)
    {
    	input_string = cmd->argv[1];
    }
    else
    {
        input_string = "OO";
    }

    /* Initialize movement to the right. */
    bool moveRight = true;

    /** 
     * Print LIMIT number of lines with each line consisting of printPos number
     * of spaces and then the input_string. Continue printing only if the user
     * has not signalled to stop execution.
     */
    int printPos = 0;
    int i = 0;
    for(; i < LIMIT && !stop; i++)
    {
        /* Add all necessary spaces and print the spaces and input_string */
    	int j = 0;
    	for(; j < printPos; j++)
    	{
    		snprintf(&str_buff[j], BUFF_SIZE - (j + strlen(input_string)), " ");
    	}
    	snprintf(&str_buff[printPos], 
    		BUFF_SIZE - (printPos + strlen(input_string)), input_string);
    	snprintf(&str_buff[printPos + strlen(input_string)], 
    		BUFF_SIZE - (printPos + 1 + strlen(input_string)), "\n");
    	write(STDOUT_FILENO, str_buff, strlen(str_buff));

        /* Increment or decrement printPos depending on movement direction */
    	printPos += (moveRight ? 1 : -1);

        /* Once you get to one side of the screen, switch directions */
    	if(i != 0 && i % WIDTH == 0)
    	{
    		moveRight = !moveRight;
    	}

        /* Wait between printing each line */
    	usleep(DELAY);
    }

    /* If the user has let the plugin finish execution, print a fun message
     * using a random number generator.
     */
    if(!stop)
    {
		snprintf(str_buff, BUFF_SIZE, "\nCalculating chill to pull ratio. . .");
		write(STDOUT_FILENO, str_buff, strlen(str_buff));

        /** Seed random number generator with time */
        srand(time(NULL));

		int chill = (int) (rand() / (double)RAND_MAX * 6.0);
		int pull = (int) (rand() / (double)RAND_MAX * 6.0);

		sleep(2);

		snprintf(str_buff, BUFF_SIZE, "\n%d:%d\n\n", chill, pull);
		write(STDOUT_FILENO, str_buff, strlen(str_buff));

        if(chill == 5 && pull == 5)
        {
            snprintf(str_buff, BUFF_SIZE, "McCooooooooyyyyyy!\n\n");
            write(STDOUT_FILENO, str_buff, strlen(str_buff));
        }
    }

    /* Reset initial signal handler*/
    sa_sigint.sa_handler = old_sigint;
    sa_sigtstp.sa_handler = old_sigtstp;
    sigaction(SIGINT, &sa_sigint, NULL);
    sigaction(SIGTSTP, &sa_sigtstp, NULL);

    return true;
}

/**
 * Print a disapproving message since the user is terminating yolo's execution
 * prior to completon.
 */
static void yolo_handle(int signo)
{
	snprintf(str_buff, BUFF_SIZE, "\nBro, do you even YOLO?\n");
	write(STDOUT_FILENO, str_buff, strlen(str_buff));

	stop = true;
}

struct esh_plugin esh_module = {
    .rank = 1,
    .init = init_plugin,
    .process_builtin = yolo_run
};