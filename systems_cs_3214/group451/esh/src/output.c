
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "esh-sys-utils.h"
#include "output.h"
#include "utils.h"

/* Prints the job JID surrounded with brackets */
static void print_jid(struct esh_pipeline * job);

/* Prints the status of the job */
static void print_status(struct esh_pipeline * job);

/* Prints the process group ID of the job */
static void print_pgid(struct esh_pipeline * job);

/* Prints commands that were typed into the job */
static void print_args(struct esh_pipeline * job);

void print_msg(char *str) {
	write(STDOUT_FILENO, str, strlen(str));
}

void print_jobs(void) {
	// Check if signal was previously blocked.
	bool prev_blocked = esh_signal_is_blocked(SIGCHLD);
	block_sigchld(prev_blocked);

	// Get a pointer to the list of jobs.
	struct list * jobs = get_jobs();

	// Get the first element of the jobs list.
	struct list_elem * job_elem = list_begin(jobs);
	// Iterate through all the jobs
	for (; job_elem != list_end(jobs); job_elem = list_next(job_elem)) {
		struct esh_pipeline * job_struct = 
			list_entry(job_elem, struct esh_pipeline, elem);

		print_single_job(job_struct, true);
	}

	trim_jobs_list(false);

	unblock_sigchld(prev_blocked);
}

void print_bg_stop_job(struct esh_pipeline * job) {
	// Check if signal was previously blocked.
	bool prev_blocked = esh_signal_is_blocked(SIGCHLD);
	block_sigchld(prev_blocked);

	print_jid(job);

	print_msg("+ ");

	print_status(job);

	print_msg(" ");

	print_args(job);

	print_msg("\n");

	unblock_sigchld(prev_blocked);
}

void print_bg_cont_job(struct esh_pipeline * job) {
	// Check if signal was previously blocked.
	bool prev_blocked = esh_signal_is_blocked(SIGCHLD);
	block_sigchld(prev_blocked);

	print_args(job);

	print_msg("\n");

	unblock_sigchld(prev_blocked);
}

void print_stopped_job(struct esh_pipeline * job) {
	// Check if signal was previously blocked.
	bool prev_blocked = esh_signal_is_blocked(SIGCHLD);
	block_sigchld(prev_blocked);

	print_msg("\n");

	print_jid(job);

	print_msg("+ ");

	print_status(job);

	print_msg(" ");

	print_args(job);

	print_msg("\n");

	unblock_sigchld(prev_blocked);
}

void print_fg_job(struct esh_pipeline * job) {
	// Check if signal was previously blocked.
	bool prev_blocked = esh_signal_is_blocked(SIGCHLD);
	block_sigchld(prev_blocked);

	print_args(job);

	print_msg("\n");

	unblock_sigchld(prev_blocked);
}

void print_bg_job(struct esh_pipeline * job) {
	// Check if signal was previously blocked.
	bool prev_blocked = esh_signal_is_blocked(SIGCHLD);
	block_sigchld(prev_blocked);

	// Print job id.
	print_jid(job);

	print_msg(" ");

	// Print pid of last process of pipeline
	print_pgid(job);

	print_msg("\n");

	unblock_sigchld(prev_blocked);
}

void print_single_job(struct esh_pipeline * job, bool print_status_flag) {
	// Check if signal was previously blocked.
	bool prev_blocked = esh_signal_is_blocked(SIGCHLD);
	block_sigchld(prev_blocked);

	// Print JID
	print_jid(job);

	print_msg(" ");

	if (print_status_flag) {
		// Print the job status
		print_status(job);
		print_msg(" ");
	}

	// Print the commands of the job.
	print_args(job);

	print_msg("\n");

	unblock_sigchld(prev_blocked);
}

static void print_jid(struct esh_pipeline * job) {
	char str_buff[BUFF_SZ];
	// Print the JID
	snprintf(str_buff, BUFF_SZ, "[%d]", job->jid);
	print_msg(str_buff);
}

static void print_status(struct esh_pipeline * job) {
	char str_buff[BUFF_SZ];
	switch (job->status) {
		case BACKGROUND: {
			snprintf(str_buff, BUFF_SZ, "Running");
			print_msg(str_buff);
			break;
		}
		case DONE: {
			snprintf(str_buff, BUFF_SZ, "Done");
			print_msg(str_buff);
			break;
		}
		case STOPPED: {
			snprintf(str_buff, BUFF_SZ, "Stopped");
			print_msg(str_buff);
			break;
		}
		case KILLED: {
			snprintf(str_buff, BUFF_SZ, "Killed");
			print_msg(str_buff);
			break;
		}
		case FOREGROUND: {
			snprintf(str_buff, BUFF_SZ, "Foreground");
			print_msg(str_buff);
			break;
		}
		default: {
			perror("Job status is unknown");
			exit(1);
		}
	}
}

static void print_pgid(struct esh_pipeline * job) {
	char str_buff[BUFF_SZ];
	snprintf(str_buff, BUFF_SZ, "%d", job->pgrp);
	print_msg(str_buff);
}

static void print_args(struct esh_pipeline * job) {
	print_msg("( ");
	char str_buff[BUFF_SZ];
	// Get the first command inside of the job.
	struct list_elem * cmd_elem = list_begin(&job->commands);
	// Iterate through all the commands in the job
	for (; cmd_elem != list_end(&job->commands); 
			cmd_elem = list_next(cmd_elem)) {
		struct esh_command * cmd_struct = 
			list_entry(cmd_elem, struct esh_command, elem);

		// Print the process name and arguments.
		char **p = cmd_struct->argv;
		while (*p) {
			snprintf(str_buff, BUFF_SZ, "%s ", *p++);
			print_msg(str_buff);
		}

		// Print the input redirection
		if (cmd_struct->iored_input) {
			snprintf(str_buff, BUFF_SZ, "< %s ", cmd_struct->iored_input);
			print_msg(str_buff);
		}

		// Print the output redirection
		if (cmd_struct->iored_output) {
			snprintf(str_buff, BUFF_SZ, "%s %s ", 
				cmd_struct->append_to_output ? ">>" : ">", 
				cmd_struct->iored_output);
			print_msg(str_buff);
		}

		// Print the pipeline character
		if (list_next(cmd_elem) != list_end(&job->commands)) {
			snprintf(str_buff, BUFF_SZ, "| ");
			print_msg(str_buff);
		}
	}
	print_msg(")");
}
