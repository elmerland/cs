ELmer Landaverde
ID: 9054-91691

How to invoke the program:
To compile the program run the command "make". This will compile and write the
executable to a file named "parse".

To run the program run the command "./parse". This will run the program and read
input from the console. Use input redirection to feed inpu from a file (e.g.
"./parse < file.txt"). Alternatively you can run "make run" which will run the
programan and feed it the information in the "data.txt" file.

What works and what doesn't
The parser expects correct expressions, so it does not handle invalid syntax. It
reads one line at a time until it reaches the end of file. Each line must end
with either a new line or an EOF.
