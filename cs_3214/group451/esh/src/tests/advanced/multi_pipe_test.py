# This test is for multiple jobs in the same command line.

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

# set timeout for all following 'expect*' calls to 2 seconds
c.timeout = 2

# ensure that shell prints expected prompt
assert c.expect(def_module.prompt) == 0, "Shell did not print expected prompt"

# Send command
c.sendline("echo Hello world; echo This is shell")

assert c.expect("Hello world") == 0, "Fail"

assert c.expect("This is shell") == 0, "Fail"


c.sendline("echo Hello world | wc; echo This is shell | wc")

assert c.expect("1       2      12") == 0, "Fail"
assert c.expect("1       3      14") == 0, "Fail"

# run program sleep
c.sendline("sleep 30 &; echo Five guys | wc")

#Used to get the jobid and pid of the sleep process
(jobid, pid) = shellio.parse_regular_expression(c, def_module.bgjob_regex)

assert c.expect("1       2      10") == 0, "Fail"

# ensure that shell prints expected prompt
assert c.expect(def_module.prompt) == 0, "Shell did not print expected prompt"

c.sendline("exit")

shellio.success()
