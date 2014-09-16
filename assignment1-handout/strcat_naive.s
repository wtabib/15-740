	.file	"strcat_naive.c"
	.text
.globl my_strcat
	.type	my_strcat, @function
my_strcat:
.LFB22:
	.cfi_startproc
	movq	%rdi, %rax
	movq	%rdi, %rdx
	cmpb	$0, (%rdi)
	je	.L3
.L9:
	addq	$1, %rdx
	cmpb	$0, (%rdx)
	jne	.L9
.L3:
	movzbl	(%rsi), %ecx
	testb	%cl, %cl
	je	.L5
.L8:
	movb	%cl, (%rdx)
	addq	$1, %rdx
	addq	$1, %rsi
	movzbl	(%rsi), %ecx
	testb	%cl, %cl
	jne	.L8
.L5:
	rep
	ret
	.cfi_endproc
.LFE22:
	.size	my_strcat, .-my_strcat
	.ident	"GCC: (Ubuntu/Linaro 4.4.4-14ubuntu5.1) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
