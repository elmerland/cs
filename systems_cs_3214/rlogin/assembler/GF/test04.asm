# Test case 4
#   R-type instructions
#   some I-type instructions
#   references into data segment
#   loop
#
.text
main:
        la     $t0, x
        lw     $t0, 0($t0)
        la     $t1, y
        lw     $t1, 0($t1)
begin:
        and    $t6, $t0, $t1
        slti   $t7, $t6, 0
        addi   $t3, $t3, 1
        srl    $t0, $t0, 1
        beq    $t7, $zero, begin

        la     $t0, y
        sw     $t3, 0($t3)
        addi   $v0, $zero, 10
        syscall
.data
x:      .word   47832
mask:   .word   1
y:      .word   16
