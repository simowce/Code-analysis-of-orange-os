SELECTOR_KERNEL_CS	equ	8

extern	cstart
extern	gdt_ptr

[section .bss]
stack_space	resb	2 * 1024
stack_top:

[section .text]

global _start

_start:
	mov	ah, 0Fh
	mov	al, 'K'
	mov	[gs : ((80 * 1 + 39) * 2)], ax

	mov	esp, stack_top

	sgdt	[gdt_ptr]
	call	cstart
	lgdt	[gdt_ptr]

	jmp	SELECTOR_KERNEL_CS:csinit
csinit:
	push	0
	popfd

	hlt
