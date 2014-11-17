#include <stdio.h>

#include <stdio.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "plugin.h"

void plugin_process_raw_cmdline(char** cmdline)
{
	struct list_elem * plugin_elem = list_begin(&esh_plugin_list);

	for (; plugin_elem != list_end(&esh_plugin_list); 
			plugin_elem = list_next(plugin_elem)) {
		struct esh_plugin *plugin_struct = 
			list_entry(plugin_elem, struct esh_plugin, elem);

		// Check if plugin supports raw command line.
		if (plugin_struct->process_raw_cmdline == NULL)
			continue;

		// Run raw command line processing
		if(plugin_struct->process_raw_cmdline(cmdline)) {
			// printf("Changing raw cmdline...\n");
			return;
		}
	}
}

void plugin_process_pipeline(struct esh_pipeline * pipe)
{
	struct list_elem * plugin_elem = list_begin(&esh_plugin_list);

	for (; plugin_elem != list_end(&esh_plugin_list); 
			plugin_elem = list_next(plugin_elem)) {
		struct esh_plugin *plugin_struct = 
			list_entry(plugin_elem, struct esh_plugin, elem);

		if (plugin_struct->process_pipeline == NULL)
			continue;

		// Run the process pipeline function in the plugin.
		if(plugin_struct->process_pipeline(pipe))
		{
			// printf("Changing pipeline...\n");
			return;
		}
	}
}

bool plugin_process_builtin(struct esh_command * cmd)
{
	struct list_elem * plugin_elem = list_begin(&esh_plugin_list);

	for (; plugin_elem != list_end(&esh_plugin_list); 
			plugin_elem = list_next(plugin_elem)) {
		struct esh_plugin *plugin_struct = 
			list_entry(plugin_elem, struct esh_plugin, elem);

		if (plugin_struct->process_builtin == NULL)
			continue;

		// int fd_out = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR |
		// S_IWUSR | S_IRGRP | S_IROTH);

		// dup2(fd_out, 1);
		// close(fd_out);

		// Run the process builtin function in the plugin.
		if(plugin_struct->process_builtin(cmd))
		{
			// printf("Running builtin...\n");
			return true;
		}
	}
	return false;
}

void plugin_pipeline_forked(struct esh_pipeline * pipe) {
	struct list_elem * plugin_elem = list_begin(&esh_plugin_list);

	for (; plugin_elem != list_end(&esh_plugin_list); 
			plugin_elem = list_next(plugin_elem)) {
		struct esh_plugin *plugin_struct = 
			list_entry(plugin_elem, struct esh_plugin, elem);

		if (plugin_struct->pipeline_forked == NULL)
			continue;

		// Run the pipeline fork function in the plugin.
		plugin_struct->pipeline_forked(pipe);
	}
}

void plugin_command_status_change(struct esh_command * cmd, int status) {
	struct list_elem * plugin_elem = list_begin(&esh_plugin_list);

	for (; plugin_elem != list_end(&esh_plugin_list); 
		plugin_elem = list_next(plugin_elem)) {
		struct esh_plugin *plugin_struct = 
			list_entry(plugin_elem, struct esh_plugin, elem);

		if (plugin_struct->command_status_change == NULL)
			continue;

		// Run the command status change insisde of the plugin.
		if(plugin_struct->command_status_change(cmd, status))
		{
			// printf("Changing pipeline...\n");
			return;
		}
	}
}
