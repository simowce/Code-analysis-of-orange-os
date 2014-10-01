org	0x100


	mov	ax, 0B800h
	mov	gs, ax
	mov	ah, 0Fh
	mov	al, 'w'
	mov	[gs:((80 * 0 + 39) * 2)], ax
	jmp	$
