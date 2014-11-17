Student Information
===================
Elmer Landaverde	elmer@vt.edu
Scott Sines			ssines@vt.edu

How to execute the shell
========================
To execute the shell without plugins use the following command:

./esh

To execute the shell with plugins use the following command:

./esh -p plugins

Important Notes
===============
<Any important notes about your system>

Our project is structured in the following manner:

-	esh.c and esh.h

	These files have all the main functions that launch the shell and keep
	getting a new prompt after the foreground process has completed. In here
	is where all signal processing is done and the jobs list is kept. It is also
	in charge of forking and executing new processes or pipelines.

-	output.c and output.h
	
	These files contain all the functions that print formatted information to 
	terminal. All the print statements are done by using a combination of the
	re-entrant safe snprintf and write functions. Every time something is
	printed to the terminal one of the functions of these files is used.

-	utils.c and utils.h
	
	These two files contain functions that facilitate the block and unblocking
	of the child signal. They are convenience functions to avoid repeating
	code across the shell.

-	builtin.c and builtin.h
	
	These two files contain all the functions for the built-in commands. Every
	time a built-it command is detected by the esh.c file and function inside
	of the builtin.c file is called. This is where all the built-in
	functionality is implemented.

-	plugin.c and plugin.h
	
	These two files contain functions that launch plugins at specific points in
	the cycle of processing a new command line. For example right after the
	raw command line is obtained the plugin_process_raw_command_line function
	is called. This function will iterate over all the loaded plugins and notify
	any that process the raw command line.

Assumptions:

-	We assume both built in commands and plugins will never be called with
	pipelines or input/output redirection. They will only work as stand alone
	commands (e.g. "jobs #", "yolo WORD"). We also assume that these commands
	will only be run in the foreground and not in the background.

Tests:

-	To run the tests for out project simply use the command below
	
	python stdriver.py -vabp plugins/

	This will run the basic, advance and plugin tests.


Description of Base Functionality
=================================

Functionality: Jobs
-------------------

In our esh.c file the list structure jobs is declared. When the main function
runs, it initializes the jobs list to an empty list. In addition to the jobs
list there is a job_count integer variable that keeps track of the highest
assigned JID. When the first command is executed the jobs list will be empty and
job_count will be equal to zero.

The rules for adding and removing jobs from the jobs list are as follows:

-	Only background jobs can be added to the jobs list. If at any time a
	background job is moved to the foreground it has to be removed from the jobs
	list.

-	When a job is added to the jobs list, the JID of the new job will be
	checked. If the JID is equal to -1 it is assumed that the job does not have
	a JID and so it will be placed at the back end of the jobs list. The
	job_counter will be increased by one and the resulting job_counter will be
	assigned as the JID for the new job.

-	If the JID is not equal to -1 then it is assumed that the new job already
	has a JID assigned to it. In this case the jobs list will be traversed until
	the first process with a JID greater than the JID of the new process is
	found. The new job will then be inserted before the process with the higher
	JID. This will ensure that the jobs list will always keep the jobs in 
	ascending order of JID front to back. An example of when this is used is
	when a job is started in the background (inserted into list), brought to the
	foreground (removed from list), and then move back to the background
	(inserted into list). In this case, the job should retain its original JID.

-	When a job is removed from the jobs list it will simply be popped from the
	jobs list. NOTE: It is the responsibility of the calling function to free
	the pipeline structure if it will no longer be used.

What follows is a description of how the jobs list is kept updated with every
new command that is executed:

When a new command is received the process_command_line function is called. This
function iterates through all the pipeline structures that have being created
from the raw input of the command line. Each pipeline popped from the list of
pipes and is then sent to either the foreground or the background. If it is a
foreground job, a pointer of the pipeline structure is passed to the fg_job 
variable. If it is a background job then it is added to the jobs list using the
add_job function. A foreground job by default will have a JID of -1, whereas a
background job will be assigned a new JID as soon as it is added to the jobs
list. A new background job will always be added to the back of the jobs list.

After the new job has being either added to the jobs list or set as the
foreground process it will then be passed on to the pipe_handler function. This
function will then run all of the processes inside of the pipe and link them
together. The pipe_handler function will also populate the pgrp and pid fields
of both the pipeline and each command inside the pipeline. This will enable the
retrieval of a job by specifying either the JID or pgrp of the job or the pid
to retrieve a specific command.

After the pipeline has being executed the shell will return to the
process_command_line function which will either wait for the foreground process
to stop or terminate, or it will print an appropriate message for when job is
sent directly to the background. After either of these actions the function will
then process to trim the jobs list by calling on the trim_jobs functions. The
trim_jobs function will look at all the jobs inside of the jobs list and will
remove any job that has terminated. A job is considered terminated if its status
is either DONE or KILLED. When a job of this type is encountered it is removed
from the jobs list by calling on the remove_job function. The pipeline structure
that belongs to that job is then freed so as to not leak resources. When the
trim_jobs function is done removing terminated jobs it then determines if the
job_counter needs to be adjusted. The job_counter will be set to the highest
JID in use. To accomplish this it will look at the last job in the jobs list and
at the foreground job, the bigger value will be set as the new job_counter.

The trim_jobs function is called in two places, once in the process_command_line
function and once in the print_jobs function. The difference between the two
calls is that when called from the process_command_line function every job that
is removed will be printed to terminal to indicate that is has terminated.

Going back to the process_command_line function, after a foreground job is
executed a call to the wait_for_fg_job is made. This function will wait
indefinitely until the foreground job is either stopped or terminated. If the
foreground job is stopped several things will happen. First the terminal state
will be saved to the pipeline corresponding to the foreground job. Next, the
job will be added to the jobs list by use of the add_job function. A message
indicating that the job has being sent to the background is then printed to
terminal. Finally the bg_job boolean inside of the pipeline structure is set to
true and the fg_job pointer is set to null. If the foreground job was instead
terminated then it will be freed up and the fg_job pointer set to null. Before
the function returns, the control of the terminal will be set back to the shell.
This will guarantee that every time a foreground process ends the terminal
regains control.

Command: jobs
-------------

The jobs command can be invoked by typing the following into the terminal:

jobs

This command will print every job that is currently being run in the background.
The information that is displayed includes the JID, status, and command line
arguments for each process. If a job has terminated in the background, its
terminated status will always be displayed at least once. This can happen either
by calling the jobs command, or after a foreground job terminates through the
call to trim_jobs_list. A second call to the jobs command will not display any
terminated job that has already been displayed. The status of each job are
determined as follows:

-	If at least one process is still running, the job status is running.
-	If all processes are done, the job status is done.
-	If all processes are either done or stopped, the job status is stopped.
-	If all processes are either done, stopped or killed, the job status is 
	stopped.
-	If all processes are either done or killed, the job status is killed.

When the shell receives any SIGCHLD from a process/command, the jobs are updated
according to the above rules.

Command: fg
-----------

The foreground command can be invoked by typing the following into the terminal:

fg <JID>

Where <JID> is the job ID of the process to be brought to the foreground. The
functionality for this command is implemented as follows:

Using the provided JID the corresponding pipeline structure is brought up by way
of calling the get_job_from_jid function. The status of this job is then
examined. If the job has terminated a message is printed to terminal and nothing
else is done. If the job has not terminated, the job is removed form the jobs
list by calling on the remove_job function. The foreground job is freed up, and
then replaced with the job specified by the JID. The bg_job variable inside the
new job is set to false, and the status is set to foreground. Next, the terminal
state of the job is restored and the control of the terminal is given to the
job. Finally, a continue signal (SIGCONT) is sent to the process group of the 
job and a message is printed to the terminal.

Two fail-safes are built into the function. The first determines if the JID does
not belong to any job in the jobs list, and the second determines if the process
group has already terminated. In either case, a message is printed to the
terminal and the function returns.

Eventually the shell returns to the process_command_line function which waits
until the newly set foreground process is stopped or terminated.

Command: bg
-----------

The background command can be invoked by typing the following into the terminal:

bg <JID>

Where <JID> is the job ID of the process to be continued in the background. The
functionality for this command is implemented as follows:

By calling on the get_job_from_jid function the pipeline that corresponds the
desired job is brought up. A continue signal (SIGCONT) is then sent to the 
process group and a message is printed to terminal.

There are two fail-safes are built into the function. One determines if the JID
does not belong to any job in the jobs list, and the other determines if the
process group has already terminated and the continue signal could not be sent.
In either case, a message is printed to the terminal indicating the error and
the function returns.

Command: kill
-------------

The kill command can be invoked by typing the following into the terminal:

kill <JID>

Where <JID> is the job ID of the process to be killed. The functionality for 
this command is implemented as follows:

As the two built in functions before, the pipeline structure is retrieved by
calling on the get_job_from_jid function. The a kill signal (SIGKILL) is sent to
the the process group of the pipeline.

There are two fail-safes built into the function. The first determines if the
JID does not belong to any job in the jobs list, and the second determines if
the process group has already terminated and the kill signal could not be sent.
In either case a message is printed to the terminal indicating the error and the
functions returns.


Command: stop
-------------

The stop command can be invoked by typing the following into the terminal:

stop <JID>

Where <JID> is the job ID of the process to be stopped. The functionality for
this command is implemented as follows:

The get_job_from_jid is used again to retrieve the pipeline structure that
belong to the specified job. Then a stop signal (SIGSTOP) is sent to the process
group.

There are two fail-safes built into the function. The first determines if the
JID does not belong to any job in the jobs list, and the second determines if
the process group has already terminated and the kill signal could not be sent.
In either case a message is printed to the terminal indicating the error and the
functions returns.

Functionality: Signals
----------------------

When the shell is initialized most signals are ignored. The only signal that is
not ignored is the SIGCHLD signal. NOTE: Right after every child process is 
forked the signals are set to their default values. The child signal (SIGCHLD)
is handled by the child_handler function. Inside of this function there is an
waitpid loop that will continue to loop as long as there are child processes
that have changed status. The moment that all the child processes with changed
status have being dealt with the loop will exit. Inside of the loop the shell
goes through several steps to update the status of all the jobs and commands.
First using the pid from the child signal and calling on the get_cmd_from_pid
function the corresponding command structure is obtained. Then several tests
are done to determine what state of the process is and the change is then
reflected on the command structure. After the command structure has being
updated, the update_job function is called so that the status of the job to 
which the command belongs can be updated as well. This is done for all processes
that have changed status.

The child_handler function will handle all signals that are delivered to the
shell when there is no foreground job running. In the case where there is a
foreground job running, the function process_command_line will wait until the
foreground job is terminated or stopped by calling on the wait_for_fg_job
function. This function waits for the foreground job in a very similar fashion
as the child_handler function. The only difference being that the
wait_for_fg_job function will wait even if no child process has changed status.
This means that the infinite loop will wait until any process changes status.
When this happens, the status change is dealt in the same way as in the
child_handler function. After the command and job have been updated, a check on
the foreground job is done by calling on the is_foreground_job_done function
which returns true only when the foreground job has being stopped or terminated.
When the foreground job is done then a check is performed to determine if it is
stopped or terminated. If it stopped, it will be sent to the background jobs
list; otherwise, the pipeline structure corresponding to the job will be freed.

Key-binding: ^C
---------------

When a new foreground command is ran the user can then type ^C into the
keyboard and this will send an interrupt signal (SIGINT) to the child process.
As a result of the child program being interrupted the shell (parent) will be 
notified with a child signal (SIGCHLD). The wait_for_fg_job function will then
deal with the change in status of the process as described above.

Because the process was running in the foreground when this process ends as a
result of typing ^C into the terminal the job will be terminated. When the job
is terminated the wait_for_fg_job function will exit from its infinite loop and
the appropriate actions will be taken to dispose of the pipeline structure 
corresponding to the recently terminated job.

Key-binding: ^Z
---------------

When a new foreground command is ran the user can then type ^Z into the
keyboard. This will result in the foreground process receiving a stop signal
(SIGTSTP) that will reach the shell (parent) in the form of a child signal
(SIGCHLD). Then the wait_for_fg_job function will deal with the change in status
of the foreground job as described above.

Because the process was running in the foreground, when this process is stopped
as a result of typing ^Z into the terminal, it will be sent to the background
jobs list and its status will be set to stopped. The wait_for_fg_job function
will then store the terminal state in the pipeline corresponding to the 
foreground job and the control of the terminal will be given to the shell.

Description of Extended Functionality
=====================================

Functionality: Pipes
--------------------

The piping functionality is handled in the pipe_handler method which takes an
esh_pipeline structure and iterates through its list of esh_commands. For a pipe
that contains multiple commands, these child processes must be piped together.
Two references to pipes, in_fd and out_fd, are maintained in the shell, one for
the STDIN and one for the STDOUT for the child process. We iterate through the
commands with a simple for-loop. Upon each iteration, a new out_fd pipe is
created as long as we are not on the last command in the pipeline since the last
command's output should stay directed to the terminal. A new child is then
forked. The pipes are then connected. If it is the first command, only the
out_fd pipe is connected to STDOUT, if it is the last command, only the in_fd
pipe is connected to STDIN; otherwise, both pipes are connected to STDOUT and
STDIN for an "interior" command. After each respective pipe is dup'd, both ends
are closed so that we do not maintain unnecessary references to the pipes. In
the parent we close the in_fd pipe since this pipe is done being used, then we
copy the out_fd pipe into the in_fd pipe for the next command so that we
maintain the proper references. For example, if cmd1 directs its output to
PIPE1, then cmd2 should read it's input from the same pipe, PIPE1. On the next
iteration of the loop, a new out_fd pipe is created and the process is repeated.

Functionality: I/O
------------------

The Input/Output functionality is handled in the command_handler. After the
pipes are set up in the pipe_handler, the command_handler takes in the command
currently being processed and checks the iored_input, iored_output, and
append_to_output flags to determine if input/output redirection is required. For
input redirection, the input file is opened with the necessary flags. This file
descriptor is then dup'd to STDIN of the process and then that file descriptor
is closed. For output redirection, we open the output file with the O_WRONLY,
O_CREAT flags and then O_APPEND or O_TRUNC depending on whether the output is to
be appended or not respectively. The file permissions are set with accordance
to a 644 chmod. This file descriptor is then dup'd to STDIN of the process and
then that file descriptor is closed. If there was an error in opening any of the
files, an appropriate error message is displayed. All input and output are now
set up and the new process can be executed.

Functionality: Exclusive Access
-------------------------------

Exclusive access is supported through the proper transfer of terminal control
and terminal states. The only difference with exclusive access is that state of
the controlling process takes up the entire terminal. Therefore, by properly
handling terminal control and states, we handle exclusive access.

When esh is started, the initial state of the shell is saved into the shell's
terminal_state member variable. Whenever the shell has control of the terminal,
the state is always in this initial state and whenever a new process is started,
they inherit this initialized state. At the beginning of our pipe_handler, we
set the state of the new process to the initial shell state regardless of if it
is a foreground process or not. If the new job is a foreground job, we give the
job terminal control using tcsetpgrp after forking the new process. We do this
both in the child and parent process since we do not know which process will be
executed first. When a foreground process terminates or is stopped we restore
the shell's initial state and restore control to the shell. Whenever a
foreground process is stopped we use the esh_sys_tty_save method to save the
current state into the foreground's job struct. Whenever a process is brought to
the foreground, it's saved terminal state is restored using the 
esh_sys_tty_restore method.

List of Plugins Implemented
===========================

WRITTEN BY OUR GROUP
====================

Plugin #1 Name: history

How to execute the plugin
-------------------------

In order to use the history plugin, you must support plugins that use the
process_raw_cmdline function pointer. This will allow the plugin to keep track
of the command history. Also you can clear the command history by using the
"clean history" command.


Description of Functionality
----------------------------

This plugin keeps history of the commands you have entered into the command line
and allows you to use the UP and DOWN arrow keys to access commands that you
have entered in the past. The history is persistent from one session to another
since it stores the command history in a hidden file named ".esh_history" which
is loaded upon initialization of the plugin. Up to 100 commands are stored in
the history and consecutive duplicate commands are not stored. You can use the
"clean history" command in order to erase the current history.

-------------------------------------------------------------------------------

Plugin #2 Name: piglatin

How to execute the plugin
-------------------------

The piglatin plugin is executed as follows:

piglatin ... WORD ...

First write "piglatin" which is the name of the plugin. Next the user can write
as many words as needed. The plugin will take an unlimited number of arguments
and parse all of them into pig latin.

Description of Functionality
----------------------------

The plugin will analyze one word at a time and apply the following rules:

-	If the word starts with vowel then "way" will be appended to the end of the
	word. Example: apple -> appleway, exit -> exitway

-	If the word starts with a consonant then everything up to the first vowel
	will be cut from the word and put at the end followed by "ay". In this case
	the letter 'y' counts as a vowel. Example: phil -> ilphay, scott -> ottscay

-------------------------------------------------------------------------------

Plugin #3 Name: yolo

How to execute the plugin
-------------------------

The yolo plugin is executed as follows:

yolo <input_string>

First write "yolo" which is the name of the plugin. Next the user has the option
to specify an input_string for the function.

Description of Functionality
----------------------------

The plugin will take the user's input_string (default input_string is 'OO') and
display it repeatedly on the screen following a snake-like pattern. The printing
will continue for ten seconds after which a random number generator will be used
to generate a fun message. The user also has the option to stop the execution of
the plugin with Ctrl-Z or Ctrl-C. The handlers for these functions are reset
during the execution of the plugin and they are reset back to their initial
states prior to exiting the plugin.

WRITTEN BY OTHERS
=================
<Group name> minahm92+dmoore09
<Plugin name> 455_duel

<Group name> minahm92+dmoore09
<Plugin name> 455_random

<Group name> minahm92+dmoore09
<Plugin name> 455_morse

<Group name> dg2269+naids
<Plugin name> group429_session_time

<Group name> dg2269+naids
<Plugin name> group429_percent_foreground

<Group name> twide23+jeh2158
<Plugin name> group413_seeya

<Group name> twide23+jeh2158
<Plugin name> group413_yomama

<Group name> twide23+jeh2158
<Plugin name> group413_compliment

<Group name> davidk47+shah18
<Plugin name> group415_harrypotter

<Group name> davidk47+shah18
<Plugin name> group415_zodiac

<Group name> davidk47+shah18
<Plugin name> group415_war

<Group name> bwright5+bggreen
<Plugin name> group432_reverseEcho

<Group name> bwright5+bggreen
<Plugin name> group432_toCelsius

<Group name> samm+kaiscs94
<Plugin name> group454_lua

<Group name> alexmc+sarahn93
<Plugin name> group423_custom_prompts

<Group name> btr274+elena
<Plugin name> group456_dec2bin

<Group name> btr274+elena
<Plugin name> group456_dec2hex

<Group name> dnguy06+crowdern
<Plugin name> group439_colour_plugin

<Group name> dnguy06+crowdern
<Plugin name> group439_superhero

<Group name> zphricz+rgarland
<Plugin name> group446_glob_expansion

<Group name> zphricz+rgarland
<Plugin name> group446_familiar_prompt

<Group name> lavakian+rkiel
<Plugin name> group434_toMilitary

<Group name> lavakian+rkiel
<Plugin name> group434_toStandard

<Group name> sfentonx + awex
<Plugin name> group407_starwars

<Group name> paulyea+broble
<Plugin name> group461_envcontrol

<Group name> paulyea+broble
<Plugin name> group461_hex

<Group name> paulyea+broble
<Plugin name> group461_binary

<Group name> paulyea+broble
<Plugin name> group461_workdir

<Group name> mtedesco+mrchang
<Plugin name> group436_1337

<Group name> mtedesco+mrchang
<Plugin name> group436_oddeven

<Group name> aemoore+danh0902
<Plugin name> group422_converter

<Group name> aemoore+danh0902
<Plugin name> group422_binHex

<Group name> aemoore+danh0902
<Plugin name> group422_reverseString

<Group name> aemoore+danh0902
<Plugin name> group422_chdir

<Group name> aemoore+danh0902
<Plugin name> group422_summation

<Group name> zsheng2+garyli
<Plugin name> group#417_cal

<Group name> tdinkel+watchful
<Plugin name> group442_areyousure

<Group name> spruett3+dommap
<Plugin name> group457_killbg

<Group name> spruett3+dommap
<Plugin name> group457_glob

<Group name> rich4756+pavelhov
<Plugin name> lawofcosine

<Group name> rich4756+pavelhov
<Plugin name> capital

<Group name> aguiar+ahuja
<Plugin name> group401_pkdx

<Group name> ajn123+tpreiss
<Plugin name> group424_impossibleQuiz

<Group name> ajn123+tpreiss
<Plugin name> group424_tempF

<Group name> ajn123+tpreiss
<Plugin name> group424_dosequis

<Group name> sbrink+brycel
<Plugin name> group416_timer

<Group name> sbrink+brycel
<Plugin name> group416_dom

<Group name> sbrink+brycel
<Plugin name> group416_startedFromTheBottom

<Group name> sbrink+brycel
<Plugin name> group416_ohHello

<Group name> sbrink+brycel
<Plugin name> group416_stringToBinary

<Group name> sbrink+brycel
<Plugin name> group416_intToBinary

<Group name> sbrink+brycel
<Plugin name> group416_iHeartSystems

<Group name> adahan+skhsu91
<Plugin name> group403_doge
