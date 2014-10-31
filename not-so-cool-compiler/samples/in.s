	.section	__TEXT,__text,regular,pure_instructions
	.globl	_IO_abort
	.align	4, 0x90
_IO_abort:                              ## @IO_abort
## BB#0:
	subq	$24, %rsp
	movq	%rsi, %rax
	movq	%rdi, 16(%rsp)
	movq	%rax, 8(%rsp)
	movq	___stderrp@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rdi
	leaq	LC(%rip), %rsi
	movq	%rax, %rdx
	xorl	%eax, %eax
	callq	_fprintf
	movl	$1, %edi
	xorl	%eax, %eax
	callq	_exit
	addq	$24, %rsp
	ret

	.globl	_method_out
	.align	4, 0x90
_method_out:                            ## @method_out
## BB#0:
	subq	$24, %rsp
	movq	%rdi, 8(%rsp)
	movq	%rsi, (%rsp)
	leaq	LC(%rip), %rdi
	xorl	%eax, %eax
	callq	_printf
	movq	8(%rsp), %rax
	movq	%rax, 16(%rsp)
	addq	$24, %rsp
	ret

	.globl	_IO_in
	.align	4, 0x90
_IO_in:                                 ## @IO_in
## BB#0:
	pushq	%r15
	pushq	%r14
	pushq	%rbx
	subq	$1040, %rsp             ## imm = 0x410
	movq	%rdi, 1024(%rsp)
	movl	$0, 1020(%rsp)
	movq	___stderrp@GOTPCREL(%rip), %r15
	xorl	%ebx, %ebx
	leaq	LC1(%rip), %r14
	jmp	LBB2_1
	.align	4, 0x90
LBB2_3:                                 ##   in Loop: Header=BB2_1 Depth=1
	movq	(%r15), %rdi
	movq	%r14, %rsi
	movb	%bl, %al
	callq	_fprintf
	movl	$1, %edi
	movb	%bl, %al
	callq	_exit
LBB2_1:                                 ## =>This Inner Loop Header: Depth=1
	callq	_getchar
	movb	%al, 1019(%rsp)
	cmpb	$10, %al
	je	LBB2_4
## BB#2:                                ##   in Loop: Header=BB2_1 Depth=1
	movslq	1020(%rsp), %rax
	movb	1019(%rsp), %cl
	movb	%cl, 8(%rsp,%rax)
	movl	1020(%rsp), %eax
	incl	%eax
	movl	%eax, 1020(%rsp)
	cmpl	$1000, %eax             ## imm = 0x3E8
	jl	LBB2_1
	jmp	LBB2_3
LBB2_4:
	movslq	1020(%rsp), %rax
	movb	$0, 8(%rsp,%rax)
	leaq	8(%rsp), %rax
	movq	%rax, 1008(%rsp)
	movq	%rax, 1032(%rsp)
	addq	$1040, %rsp             ## imm = 0x410
	popq	%rbx
	popq	%r14
	popq	%r15
	ret

	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
## BB#0:
	movl	%edi, -8(%rsp)
	movl	$0, -4(%rsp)
	xorl	%eax, %eax
	ret

	.section	__TEXT,__const
LC:                                     ## @"\01LC"
	.asciz	 "%s"

	.align	4                       ## @"\01LC1"
LC1:
	.asciz	 "Error: input cannot exceed 1000 chars"


.subsections_via_symbols
