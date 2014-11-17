# Test case 2
#   R-type instructions
#   some I-type instructions
#   some pseudos
#   no references into data segment
#
.text
main:
        li     $s0, 10
        li     $s1, 20

        sll    $s2, $s0, 23
        srl    $s3, $s1,  9

        addiu  $s4, $s3, 18763
        or     $s5, $s4, $s3
        and    $s6, $s5, $s4

        li     $v0, 10
        syscall
.data
x:      .word    255
