# .bashrc supplied for Assignment 2
#
# Create an uncompressed tar file containing your final version of this
# file and of the associated .bash_profile and submit that tar file to
# the Curator by the posted deadline.
#
# Student name: Elmer Landaverde
# Student PID: elmer@vt.edu
#
# Set alias(es) here:
alias list='ls -laigG --time-style=long-iso'
alias brc='source .bashrc'
alias bpr='source .bash_profile'

# I strongly suggest you not modify anything below this comment!
#
# If not running interactively, don't do anything
[ -z "$PS1" ] && return

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

