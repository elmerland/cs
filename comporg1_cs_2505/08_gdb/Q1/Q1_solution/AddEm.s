	.file	"AddEm.c"
	.section	.rodata
.LC0:
	.string	"Sum: %d\n"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$48, %esp
	movl	$5, 40(%esp)
	movl	$32, 20(%esp)
	movl	$16, 24(%esp)
	movl	$64, 28(%esp)
	movl	$4, 32(%esp)
	movl	$8, 36(%esp)
	movl	40(%esp), %eax
	movl	%eax, 4(%esp)
	leal	20(%esp), %eax
	movl	%eax, (%esp)
	call	AddEm
	movl	%eax, 44(%esp)
	movl	$.LC0, %eax
	movl	44(%esp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
.globl AddEm
	.type	AddEm, @function
AddEm:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -8(%ebp)
	movl	$0, -4(%ebp)
	movl	12(%ebp), %eax
	sall	$4, %eax
	addl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp	.L4
.L5:
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	addl	%eax, -8(%ebp)
	movl	8(%ebp), %eax
	addl	$1, %eax
	movl	%eax, 8(%ebp)
.L4:
	movl	8(%ebp), %eax
	cmpl	-4(%ebp), %eax
	jb	.L5
	movl	-8(%ebp), %eax
	leave
	ret
	.size	AddEm, .-AddEm
	.ident	"GCC: (GNU) 4.4.4 20100726 (Red Hat 4.4.4-13)"
	.section	.note.GNU-stack,"",@progbits
