#ifndef PLUGIN
#define PLUGIN

#include "esh.h"

/* Run first plugin that processes raw command line if they exist */
/**
 * Runs the first plugin that processes the raw command line.
 * @param  cmdline A pointer to the raw command line.
 */
void plugin_process_raw_cmdline(char ** cmdline);

/**
 * Runs the first plugin that processes the pipeline.
 * @param  pipe A pointer to the pipeline to be executed.
 */
void plugin_process_pipeline(struct esh_pipeline * pipe);

/**
 * Runs the first plugin that executes the user's command.
 * @param  cmd A pointer to the comand to be executed
 * @return     True if the plugin whishes processing to stop.
 */
bool plugin_process_builtin(struct esh_command * cmd);

/**
 * Runs the first plugin that processes the pipeline after all
 * commands have being forked and after the JID, PID and pgrp fields
 * have being set.
 * @param  job A pointer to the pipeline that was just forked.
 */
void plugin_pipeline_forked(struct esh_pipeline * job);

/**
 * Runs the first plying the responds to a change in a command status.
 * @param  cmd    A pointer to the command structure that changed status.
 * @param  status The value returned by waitpid().
 */
void plugin_command_status_change(struct esh_command * cmd, int status);

#endif
