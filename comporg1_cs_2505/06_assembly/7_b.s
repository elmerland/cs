	.file	"hw06b.c"
	.text
.globl func
	.type	func, @function
func:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -4(%ebp)
.L2:
	movl	8(%ebp), %eax
	imull	8(%ebp), %eax
	movl	%eax, 8(%ebp)
	addl	$2, -4(%ebp)
	movl	-4(%ebp), %eax
	cmpl	8(%ebp), %eax
	jle	.L2
	leave
	ret
	.size	func, .-func
	.ident	"GCC: (GNU) 4.4.4 20100726 (Red Hat 4.4.4-13)"
	.section	.note.GNU-stack,"",@progbits
