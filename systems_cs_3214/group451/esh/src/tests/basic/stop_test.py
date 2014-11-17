#!/usr/bin/python
#
# stop_test: tests the stop command
# 
# Test the stop command for stopping a process by its pid.
# Requires the following commands to be implemented
# or otherwise usable:
#
#	stop, sleep
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

# spawn an instance of the shell
c = pexpect.spawn(def_module.shell, drainpty=True, logfile=logfile)
atexit.register(force_shell_termination, shell_process=c)

# set timeout for all following 'expect*' calls to 2 seconds
c.timeout = 2

# ensure that shell prints expected prompt
assert c.expect(def_module.prompt) == 0, "Shell did not print expected prompt"



# run a command
c.sendline("sleep 30 &")

# pull the jobid and pid from the background process printout
(jobid, pid) = shellio.parse_regular_expression(c, def_module.bgjob_regex)

# ensure that the shell prints the expected prompt
assert c.expect(def_module.prompt) == 0, "Shell did not print expected prompt"



#The job needs to be running when we call stop
proc_check.count_children_timeout(c, 1, 1)

# send the stop command to the process
c.sendline(def_module.builtin_commands['stop'] % jobid)

#Ensure that sleep has enough time to stop before we read its
#/proc/ pid /stat file.
time.sleep(.5)



#Ensure that sleep is now stopped in the background, and is not
#the foreground process.
assert not proc_check.check_pid_fgpgrp(pid), \
                            'Error: process is in the foreground'
assert proc_check.check_pid_status(pid, 'T'), 'Error: process not stopped'

#check the prompt prints
assert c.expect(def_module.prompt) == 0, "Shell did not print expected prompt"



c.sendline("exit");
assert c.expect_exact("exit\r\n") == 0, "Shell output extraneous characters"


shellio.success()
