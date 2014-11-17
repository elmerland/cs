#ifndef OUTPUT_H
#define OUTPUT_H

#include "esh.h"

#define BUFF_SZ (256 * sizeof(char))

/* Prints all the jobs in the job list. */
void print_jobs(void);

/* Print a single job to terminal */
void print_single_job(struct esh_pipeline * job, bool print_status);

/* Print message for when a job is sent to the background */
void print_bg_job(struct esh_pipeline * pipe);

/* Prints a message for when a job is brought back to the foreground */
void print_fg_job(struct esh_pipeline * pipe);

/* Prints a message for when a background job is stopped */
void print_stopped_job(struct esh_pipeline * job);

/* Prints a message for when a background job has being sent a SIGCONT signal */
void print_bg_cont_job(struct esh_pipeline * job);

/* Prints a message for when a background job has being sent a SIGSTOP signal */
void print_bg_stop_job(struct esh_pipeline * job);

/* Prints a message using the async-signal safe write function */
void print_msg(char *str);

#endif
