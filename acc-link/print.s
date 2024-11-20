	.text
	.file	"print.ll"
	.globl	println                         # -- Begin function println
	.p2align	4, 0x90
	.type	println,@function
println:                                # @println
	.cfi_startproc
# %bb.0:                                # %println_entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$.L__unnamed_1, %edi
	xorl	%eax, %eax
	callq	printf@PLT
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	println, .Lfunc_end0-println
	.cfi_endproc
                                        # -- End function
	.globl	printi                          # -- Begin function printi
	.p2align	4, 0x90
	.type	printi,@function
printi:                                 # @printi
	.cfi_startproc
# %bb.0:                                # %println_entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	%edi, %esi
	movl	$.L__unnamed_2, %edi
	xorl	%eax, %eax
	callq	printf@PLT
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	printi, .Lfunc_end1-printi
	.cfi_endproc
                                        # -- End function
	.globl	printiln                        # -- Begin function printiln
	.p2align	4, 0x90
	.type	printiln,@function
printiln:                               # @printiln
	.cfi_startproc
# %bb.0:                                # %println_entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	%edi, %esi
	movl	$.L__unnamed_3, %edi
	xorl	%eax, %eax
	callq	printf@PLT
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	printiln, .Lfunc_end2-printiln
	.cfi_endproc
                                        # -- End function
	.globl	prints                          # -- Begin function prints
	.p2align	4, 0x90
	.type	prints,@function
prints:                                 # @prints
	.cfi_startproc
# %bb.0:                                # %println_entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movq	%rdi, %rsi
	movl	$.L__unnamed_4, %edi
	xorl	%eax, %eax
	callq	printf@PLT
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	prints, .Lfunc_end3-prints
	.cfi_endproc
                                        # -- End function
	.globl	printsln                        # -- Begin function printsln
	.p2align	4, 0x90
	.type	printsln,@function
printsln:                               # @printsln
	.cfi_startproc
# %bb.0:                                # %println_entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movq	%rdi, %rsi
	movl	$.L__unnamed_5, %edi
	xorl	%eax, %eax
	callq	printf@PLT
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end4:
	.size	printsln, .Lfunc_end4-printsln
	.cfi_endproc
                                        # -- End function
	.type	.L__unnamed_2,@object           # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_2:
	.asciz	"%d"
	.size	.L__unnamed_2, 3

	.type	.L__unnamed_3,@object           # @1
.L__unnamed_3:
	.asciz	"%d\n"
	.size	.L__unnamed_3, 4

	.type	.L__unnamed_4,@object           # @2
.L__unnamed_4:
	.asciz	"%s"
	.size	.L__unnamed_4, 3

	.type	.L__unnamed_5,@object           # @3
.L__unnamed_5:
	.asciz	"%s\n"
	.size	.L__unnamed_5, 4

	.type	.L__unnamed_1,@object           # @4
.L__unnamed_1:
	.asciz	"\n"
	.size	.L__unnamed_1, 2

	.section	".note.GNU-stack","",@progbits
