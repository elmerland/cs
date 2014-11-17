# Test case 7
#  many strings in data segment
#  integers in data segment
#
.text
main:
        la   $a0, S1
        li   $v0,  4
        syscall

        la   $a0, S2
        li   $v0,  4
        syscall

        la   $a0, S3
        li   $v0,  4
        syscall

        la   $a0, S4
        li   $v0,  4
        syscall

        la   $a0, S5
        li   $v0,  4
        syscall

        li   $v0, 10
        syscall

.data
S1:   .asciiz   "abc"
I1:   .word     1
S2:   .asciiz   "def ghi"
I2:   .word     2
S3:   .asciiz   "jklmnopqrstuvwxyzAB"
I3:   .word     3
S4:   .asciiz   "CD"
I4:   .word     4
S5:   .asciiz   "ghijk"
I5:   .word     5
S6:   .asciiz   "a"
S7:   .asciiz   "b"
