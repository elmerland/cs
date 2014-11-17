#ifndef BUILTIN
#define BUILTIN

/**
 * Prints the list of jobs that are currently in the background.
 * @param cmd The command.
 */
void built_in_jobs(struct esh_command * cmd);

/**
 * Sends a SIGCONT to a job in the background. The job is specified
 * by its JID.
 * @param cmd The command.
 */
void built_in_bg(struct esh_command * cmd);

/**
 * Brings a process that is in the background to the foreground. It also
 * sends a SIGCONT signal to the process in case it was previously stopped.
 * @param cmd The command.
 */
void built_in_fg(struct esh_command * cmd);

/**
 * Exits the shell.
 * @param cmd The command.
 */
void built_in_exit(struct esh_command * cmd);

/**
 * Sends a SIGKILL signal to a command running in the background. The job
 * is specified by its JID.
 * @param cmd The command.
 */
void built_in_kill(struct esh_command * cmd);

/**
 * Sends a SIGSTOP signal to a command running in the background. The job
 * is specified by its JID.
 * @param cmd The command
 */
void built_in_stop(struct esh_command * cmd);

#endif
