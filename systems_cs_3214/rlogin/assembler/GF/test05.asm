# Test case 5
#   R-type instructions
#   some I-type instructions
#   array in data segment
#   loop
#
.text
main:
        la     $t0, List
        la     $t1, Sz
        lw     $t1, 0($t1)
        li     $a0, 0
        li     $a1, 0

        beq    $t1, $zero, done
begin:
        lw     $t2, 0($t0)
        add    $a0, $a0, $t2
        addi   $a1, $a1, 1
        addi   $t0, $t0, 4
        bne    $a1, $t1, begin
done:
        la     $t0, Sum
        sw     $a0, 0($t0)

        addi   $v0, $zero, 10
        syscall
.data
List:   .word   2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43
Sz:     .word   14
Sum:    .word   0
