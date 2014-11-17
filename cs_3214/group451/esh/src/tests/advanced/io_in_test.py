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

if os.path.isfile("test.txt"):
    os.remove("test.txt")

# set timeout for all following 'expect*' calls to 2 seconds
c.timeout = 2

c.sendline("wc < esh-utils.c")

time.sleep(1)

assert c.expect(" 263  726 6769\r\n") == 0, "Shell did not print expected output"

c.sendline("wc < esh-utils.c | cat")

time.sleep(1)

assert c.expect(" 263  726 6769\r\n") == 0, "Shell did not print expected output"

if os.path.isfile("test.txt"):
    os.remove("test.txt")

shellio.success()
