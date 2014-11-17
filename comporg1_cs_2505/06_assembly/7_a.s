	.file	"hw06.c"
	.text
.globl func
	.type	func, @function
func:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	12(%ebp), %eax
	movl	8(%ebp), %edx
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	cmpl	$1, %eax
	jg	.L2
	movl	12(%ebp), %eax
	movl	8(%ebp), %edx
	leal	(%edx,%eax), %eax
	movl	%eax, -4(%ebp)
	jmp	.L3
.L2:
	movl	12(%ebp), %eax
	movl	8(%ebp), %edx
	leal	(%edx,%eax), %eax
	cmpl	$10, %eax
	jle	.L4
	movl	8(%ebp), %eax
	addl	%eax, %eax
	movl	%eax, -4(%ebp)
	jmp	.L3
.L4:
	movl	12(%ebp), %eax
	addl	%eax, %eax
	movl	%eax, -4(%ebp)
.L3:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	func, .-func
	.ident	"GCC: (GNU) 4.4.4 20100726 (Red Hat 4.4.4-13)"
	.section	.note.GNU-stack,"",@progbits
