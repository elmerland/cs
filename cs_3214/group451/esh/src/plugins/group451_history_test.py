#!/usr/bin/python
#
# Block header comment
#
#
import sys, imp, atexit
sys.path.append("/home/courses/cs3214/software/pexpect-dpty/");
import pexpect, shellio, signal, time, os, re, proc_check

#Ensure the shell process is terminated
def force_shell_termination(shell_process):
	c.close(force=True)

#pulling in the regular expression and other definitions
definitions_scriptname = sys.argv[1]
plugin_dir = sys.argv[2]
def_module = imp.load_source('', definitions_scriptname)
logfile = None
if hasattr(def_module, 'logfile'):
    logfile = def_module.logfile

#spawn an instance of the shell
c = pexpect.spawn(def_module.shell + plugin_dir, drainpty=True, logfile=logfile)

atexit.register(force_shell_termination, shell_process=c)

if os.path.isfile(".esh_history"):
	os.remove(".esh_history")

c.sendline("ls -la")

# Because we have no way of knowing what the prompt is we are just expecting
# any pattern.
c.expect(".*")

c.sendline("echo Hello world")

c.expect(".*")

c.sendline("echo This is a test")

c.expect(".*")

c.sendline("ls -l")

c.expect(".*")

c.sendline("echo The final test")

c.expect(".*")

c.sendline("exit")

with open(".esh_history", "r") as history:
	lines = history.readlines()
	cmd1 = False
	cmd2 = False
	cmd3 = False
	cmd4 = False
	cmd5 = False
	for line in lines:
		print(line.strip(" \n"))
		if line.strip(" \n") == "ls -la":
			cmd1 = True
		elif line.strip(" \n") == "echo Hello world":
			cmd2 = True
		elif line.strip(" \n") == "echo This is a test":
			cmd3 = True
		elif line.strip(" \n") == "ls -l":
			cmd4 = True
		elif line.strip(" \n") == "echo The final test":
			cmd5 = True
	print(cmd1, cmd2, cmd3, cmd4, cmd5)
	assert cmd1 and cmd2 and cmd3 and cmd4 and cmd5, "Failed tests"

shellio.success()
