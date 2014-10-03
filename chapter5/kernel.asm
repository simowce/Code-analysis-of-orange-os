[section .text]

global _start

_start:
	mov	ah, 0Fh
	mov	al, 'K'
	mov	[gs : ((80 * 1 + 39) * 2)], ax
	jmp	$
