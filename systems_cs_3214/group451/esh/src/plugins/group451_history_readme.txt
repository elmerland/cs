Group Number
------------
Group 451

Student Information
-------------------
Elmer Landaverde	pid: elmer@vt.edu
Scott Sines			pid: ssines@vt.edu

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
