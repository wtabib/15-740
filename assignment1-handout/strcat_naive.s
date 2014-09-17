	.file	"strcat_naive.c"
	.text
.globl my_strcat
	.type	my_strcat, @function
my_strcat:
.LFB22:
	.cfi_startproc
	movq	%rdi, %rax # *tmp = dest
    # rdi seems to be tmp (?)
	movq	%rdi, %rdx 
	cmpb	$0, (%rdi) # *dest != 0
	je	.L3            # continue to the for loop
# This is the while loop
# here rdx is dest
.L9:
	addq	$1, %rdx   # dest++
	cmpb	$0, (%rdx) # dest != 0
	jne	.L9            # go back to the start of the while loop
.L3:
	movzbl	(%rsi), %ecx    # *dest = *src
	testb	%cl, %cl        # *src == \0
	je	.L5                 # if condition is verified break
.L8:
	movb	%cl, (%rdx)     # *dest = *src
    #incresing dest and src (that is rsi)
	addq	$1, %rdx        # dest++
	addq	$1, %rsi        # src ++
	movzbl	(%rsi), %ecx    # *dest = *src
	testb	%cl, %cl        # *src == \0
	jne	.L8                 # if condition not satisfied keep on looping in the
                            # for
.L5:
	rep
	ret                     # return
	.cfi_endproc
.LFE22:
	.size	my_strcat, .-my_strcat
	.ident	"GCC: (Ubuntu/Linaro 4.4.4-14ubuntu5.1) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
