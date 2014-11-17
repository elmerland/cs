
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "esh.h"
#include "output.h"
#include "esh-sys-utils.h"
#include "builtin.h"

void built_in_jobs(struct esh_command * cmd) {
	// Print all the bakcground jobs.
	print_jobs();

	get_fg_job()->status = DONE;
}

void built_in_bg(struct esh_command * cmd) {
	// Check if argument is valid.
	if (cmd->argv[1] == NULL) {
		print_msg("esh: bg: Missing parameter\n");
		get_fg_job()->status = DONE;
		return;
	}
	// Get command JID
	int target_jid = strtol(cmd->argv[1], (char **)NULL, 10);
	// Get pipeline structure.
	struct esh_pipeline * job_struct = get_job_from_jid(target_jid);
	if (job_struct == NULL) {
		print_msg("esh: bg: No such job\n");
		get_fg_job()->status = DONE;
		return;
	}

	// Send continue signal to process group.
	if (kill(job_struct->pgrp * -1, SIGCONT) == -1) {
		print_msg("esh: bg: Job is terminated\n");
		get_fg_job()->status = DONE;
		return;
	}

	// Print background job status.
	print_bg_cont_job(job_struct);

	get_fg_job()->status = DONE;
}

void built_in_fg(struct esh_command * cmd) {
	// Check if argument is valid.
	if (cmd->argv[1] == NULL) {
		print_msg("esh: fg: Missing parameter\n");
		get_fg_job()->status = DONE;
		return;
	}

	// Get the job id from the command line arguments
	int target_jid = strtol(cmd->argv[1], (char **)NULL, 10);
	// Get the job from the job id
	struct esh_pipeline * job_struct = get_job_from_jid(target_jid);
	if (job_struct == NULL) {
		print_msg("esh: fg: no such job\n");
		get_fg_job()->status = DONE;
		return;
	}

	// Check if job is terminated
	if (job_struct->status == DONE) {
		print_msg("esh: fg: job has terminated\n");
		get_fg_job()->status = DONE;
		return;
	}
	// Remove the job from jobs list.
	remove_job(job_struct->pgrp);
	// Free the foreground process pipeline
	esh_pipeline_free(get_fg_job());
	// Set the foreground job pointer to the new foregrund job.
	set_fg_job(job_struct);
	// Changed the background status of the foreground process.
	get_fg_job()->bg_job = false;
	get_fg_job()->status = FOREGROUND;

	// Restore the terminal state and give the terminal to the new fg process
	esh_sys_tty_restore(&job_struct->saved_tty_state);
	if (tcsetpgrp(STDIN_FILENO, job_struct->pgrp) == -1) {
		perror("Fg_command: Could not set control of terminal");
		exit(1);
	}

	// Send continue signal to foreground process in case it was stopped.
	if (kill(job_struct->pgrp * -1, SIGCONT) == -1) {
		perror("Could not send continue signal");
		exit(1);
	}

	// Print the process name and arguments.
	print_fg_job(job_struct);
}

void built_in_exit(struct esh_command * cmd) {
	// Exit the shell.
	exit(0);
}

void built_in_kill(struct esh_command * cmd) {
	// Get the JID of the process.
	int target_jid = strtol(cmd->argv[1], (char **)NULL, 10);
	// Get the pipeline structure
	struct esh_pipeline * job_struct = get_job_from_jid(target_jid);
	if (job_struct == NULL) {
		errno = EINVAL;
		perror("No job for specified JID");
		exit(1);
	}
	// Send kill signal to process group.
	if (kill(job_struct->pgrp * -1, SIGKILL) == -1) {
		perror("Could not send continue signal");
		exit(1);
	}

	get_fg_job()->status = DONE;
}

void built_in_stop(struct esh_command * cmd) {
	// Get the JID of the process.
	int target_jid = strtol(cmd->argv[1], (char **)NULL, 10);
	// Ge the pipeline structure
	struct esh_pipeline * job_struct = get_job_from_jid(target_jid);
	// Set job status.
	job_struct->status = STOPPED;
	// Print job status stopped line.
	print_bg_stop_job(job_struct);
	// Send stop signal to process group.
	if (kill(job_struct->pgrp * -1, SIGSTOP) == -1) {
		perror("Could not send stop signal");
		exit(1);
	}

	get_fg_job()->status = DONE;
}
