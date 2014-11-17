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
def_module = imp.load_source('', definitions_scriptname)
logfile = None
if hasattr(def_module, 'logfile'):
    logfile = def_module.logfile

#spawn an instance of the shell
c = pexpect.spawn(def_module.shell, drainpty=True, logfile=logfile)

atexit.register(force_shell_termination, shell_process=c)

# If test file exists, delete it.
if os.path.isfile("test.txt"):
	os.remove("test.txt")

# set timeout for all following 'expect*' calls to 2 seconds
c.timeout = 2

# start vim in the background
c.sendline("vim test.txt &")

#snag the jobid and pid of the sleep command
(jobid, pid) = shellio.parse_regular_expression(c, def_module.bgjob_regex)
print("JID 1: " + str(jobid) + ", PID 1: " + str(pid))

# Bring vim back to the foreground
c.sendline("fg " + str(jobid))

# Wait until vim is the foreground
proc_check.wait_until_child_is_in_foreground(c)

#checks the number of active child processes
#at this moment in time
proc_check.count_active_children(c, 1)

# Check the it is in fact vim that is in the foreground
assert proc_check.check_pid_fgpgrp(pid), "Error: Vim not in foreground"

# Go into insert mode and type in a line of text.
c.sendline("iHello world, this is esh. What can I help you with")

time.sleep(1)

# Send the escape character
c.sendline(u"\u001b")

time.sleep(1)

# Save the file and exit out of vim.
c.sendline(":wq\r")

time.sleep(2)

# Test if the file contents are correct.
if os.path.isfile("test.txt") == False:
	assert False, "Vim did not create new file"

with open("test.txt", "r") as test_file:
	line = test_file.readline()
	assert line == "Hello world, this is esh. What can I help you with\n", "Shell did not print expected output to file"

time.sleep(1)

# Echo hello world to a test file after vim has exited.
c.sendline("echo Hello world > test.txt")

time.sleep(1)

# Test contents of file.
with open("test.txt", "r") as test_file:
	line = test_file.readline()
	assert line == "Hello world\n", "Shell did not print expected output to file"

# Echo hello world to the terminal
c.sendline("echo Hello world")

assert c.expect("Hello world\r\n") == 0, "Shell did not print expected output"

# Delete test file.
if os.path.isfile("test.txt"):
    os.remove("test.txt")

shellio.success()
