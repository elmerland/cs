# Test case 3
#   R-type instructions
#   some I-type instructions
#   references into data segment
#
.text
main:
        la     $t0, x
        lw     $t1, 0($t0)
        la     $t1, y
        lw     $t1, 0($t1)
        add    $t2, $t1, $t0
        la     $t3, z
        sw     $t2, 0($t3)
        addi   $v0, $zero, 10
        syscall
.data
x:      .word    8
y:      .word   16
z:      .word  729
