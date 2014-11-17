# This tests a single job per command line with multiple pipes in it.

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

# Send command
c.sendline("echo Hello World | cat")

# ensure that shell prints expected prompt
assert c.expect_exact("Hello World\r\n") == 0, "Shell did not print expected output"

# Send command
c.sendline("echo Five guys is the best | wc")

# ensure that shell prints expected prompt
assert c.expect_exact("      1       5      22\r\n") == 0, "Shell did not print expected output"

# Send command
c.sendline("echo We went to five guys and it was awesome | cat | wc")

# ensure that shell prints expected prompt
assert c.expect_exact("      1       9      40\r\n") == 0, "Shell did not print expected output"

# Send command
c.sendline("echo We went to five guys and it was awesome | cat | wc | wc")

# ensure that shell prints expected prompt
assert c.expect_exact("      1       3      24\r\n") == 0, "Shell did not print expected output"

# Send command
c.sendline("echo Hello this is shell, what can i do for you? | cat | cat | cat | cat | cat")

# ensure that shell prints expected prompt
assert c.expect_exact("Hello this is shell, what can i do for you?\r\n") == 0, "Shell did not print expected output"

shellio.success()
