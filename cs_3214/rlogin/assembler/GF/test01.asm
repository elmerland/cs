# Test case 1
#   simple R-type instructions
#   no references into data segment
#
.text
main:
        addi   $s0, $zero, 10
        addi   $s1, $zero, 20

        add    $s2, $s0, $s1
        sub    $s3, $s0, $s1

        addi   $v0, $zero, 10
        syscall
.data
x:      .word    8
y:      .word   16
z:      .word  729
