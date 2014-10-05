[SECTION .text]
global	memcpy

memcpy:
	push	ebp
	mov	ebp, esp

	pop	edi
	pop	esi
	pop	ecx

	mov	edi, [ebp + 8]
	mov	esi, [ebp + 12]
	mov	ecx, [ebp + 16]

.1:
	cmp	ecx, 0
	jz	.2

	mov	al, [ds:esi]
	inc	esi

	mov	byte [es:edi], al
	inc	edi

	dec	ecx

	jmp	.1
.2:
	mov	eax, [ebp + 8]

	pop	ecx
	pop	esi
	pop	edi
	mov	esp, ebp
	pop	ebp
	ret

