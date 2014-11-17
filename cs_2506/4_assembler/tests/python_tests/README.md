MIPS Assembler Test Script
==========

Provides a few python scripts to test the CS2506 project of the MIPS assembler.
I've included a directory of the provided test inputs/outputs and may add some
more as I go. Any differences you notice can be brought up as an issue, and I'll
work to fix them.

Test file contributions are welcome, and I'll run them through my assembler
to make sure they agree.

Notes on running
----
I wrote this on Linux (ubuntu) with python 2.7. It *will not* run with python 3 (because
they aren't syntax compatible). It *should* be
platform indepedent, but I have not personally tested it on windows. If you notice any differences
when running with python2 on windows, bring them up and I'll address them.

Features
------
 - Automated make of your assembler (via *make*)
 - Runs your assembler, and tests the output against a known-correct output
 - Produces a side-by-side diff of your output vs correct output
 - Disassembles the instructions to see what you output vs. what is correct
 - Produces a score of the lines you got correct / the total lines.
 
Usage
-----
This project contains three seperate scripts:
 - diff.py
 - disassembler.py
 - tester.py
 
diff.py provides a simple side-by-side diff implementation in python, and can be invoked with

    python diff.py file-a file-b

disassembler.py works as backwards assembler in primitive form, it produces the raw instructions
which correspond to the binary representation. It can be invoked from the command line just as an 
assembler with

    python disassembler.py file.o file.dasm

tester.py provides the test interface (and is much more robust in its cmd line interface).
Fairly detailed help can be found with 
    
    python tester.py --help.
    
In short, it is invoked as

    python tester.py file.asm correct-output.o
    
however it may take a quoted UNIX-style file pattern ('\*.asm', '\*.o') which will test all .asm files in
the directory and compare it against files which match the .o pattern with the name *most similar* to the input
file. Thus if you provide '\*.asm' and '\*.o' in a directory with files p1.asm, p1.o, p2.asm, p2.o it will select
p1.o as the correct output for p1.asm, and p2.o as the correct output for p2.asm. Be careful though as it
uses edit distance to compare for similarity. Always check to make sure it guessed the right file if you
plan to use wildcards on a lot of files.

Example output from a slightly incorrect assembler is given as below,

    $ python tester.py p1.asm p1.o -e assembler.py
    
    Testing your assembler on input file p1.asm against output file p1.o
    ----------------------------------------------------------------------
    Running your assembler...
    Checking your instruction output =>
    (1, 1) => 
	    difference: real output => addi $s0, $zero, 8192
                    your output => unknown instruction with opcode 101000
    (6, 6) => 
	    difference: real output => addi $t0, $t0, 1
                    your output => addi $t2, $t0, 1
    score: 11 / 13
    
    Checking your data segment output =>
    perfect match!
    score: 5 / 5
    -----------------------------------------------------------------------

    Finished testing your assembler on 1 file(s).
    Your total score is 16 / 18

  
as you can see it will tell you what instructions should have been assembled vs. what you actually
assembled (if it can even guess, it only knows the instructions that the spec supports).

The pairs of (i, j) correspond to line numbers from the output files. The first is the correct output line
number, and the second is your line number. These will only differ if you omit or add a line randomly (and
the diff should attempt to account for that, albeit not very intelligently).

Code quality
----
Do *not* take this as an example of any good software engineering practices.
That's pretty much it.


Side notes
--------

Remember that this code nor some of the outputs I provide are the master source. Most of it was written
in between two of my classes in about 2.5 hours. If you notice anything wrong feel free to bring it up and I'll
correct the code or investigate the issue. If you have any questions about the code or algorithms I used
you can contact me via email or on github.
