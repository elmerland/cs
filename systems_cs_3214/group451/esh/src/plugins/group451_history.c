/**
 * Plugin that implements the history functionality. Use the up and down
 * arrows to traverse through the command history. The most recent 100
 * commands are stored and persist from one session to the next. History
 * commands are stored in the file ".esh_history". The user can then run
 * a <clean history> command to clear the history file.
 */

#include <stdio.h>
#include <readline/history.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "../esh.h"

/* File where the history commands are stored */
#define HIST_FILENAME ".esh_history"

/* Maximum number of history entries that can be stored */
#define MAX_HISTORY 100

/* Maximum characters that can be stored in a string buffer */
#define BUFF_SIZE 256

/**
 * Initialize the history by reading in from the history file.
 */
static bool 
init_plugin(struct esh_shell *shell)
{
	read_history(HIST_FILENAME);

    printf("Plugin 'history' initialized...\n");
    return true;
}

/**
 * When the user inputs something into the command line it is added
 * to the command history. Does not add consecutive duplicate commands
 * to the command history.
 */
static bool
add_to_history(char** cmdline)
{
	/** Check if command is a duplicate of the last command in the history */
	if(history_length == 0 || 
		history_search_pos(*cmdline, -1, history_length - 1) != history_length - 1)
	{
		/** Add command to history and write to history file */
		add_history(*cmdline);
		write_history(HIST_FILENAME);
	}

	/** Trim history and history file if necessary */
	while(history_length > MAX_HISTORY)
	{
		remove_history(0);
	}
	history_truncate_file(HIST_FILENAME, MAX_HISTORY);

	return false;
}

/**
 * When the user runs the <clean history> command, the history entries and file
 * are cleared.
 */
static bool
clean_history(struct esh_command * cmd)
{
	/** 
	 * Check that the user is attempting to run our plugin. Print a usage
	 * message if they attempt to use it incorrectly.
	 */
	if (strcmp(cmd->argv[0], "clean") != 0)
	{
        return false;
    }
    else if (cmd->argv[1] == NULL || strcmp(cmd->argv[1], "history") != 0)
    {
    	char str_buff[BUFF_SIZE];
    	snprintf(str_buff, BUFF_SIZE, 
    		"Usage:\n\t<clear history> to clear command history\n");
    	write(STDOUT_FILENO, str_buff, strlen(str_buff));

    	return true;
    }

    /** Clear the history and write the empty history to the history file */
    clear_history();
    write_history(HIST_FILENAME);

    return true;
}

struct esh_plugin esh_module = {
    .rank = 1,
    .init = init_plugin,
    .process_raw_cmdline = add_to_history,
    .process_builtin = clean_history
};
