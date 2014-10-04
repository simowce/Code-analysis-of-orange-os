disp_al:
	push	ecx
	push	edx
	push	edi

	mov	edi, [disp_pos]

	mov	ah, 0Fh
	mov	dl, al
	shr	al, 4
	mov	ecx, 2
.begin:
	and	al, 01111b
	cmp	al, 9
	ja	.1
	add	al, '0'
	jmp	.2
.1:
	sub	al, 0Ah
	add	al, 'A'
.2:
	mov	[gs:edi], ax
	add	edi, 2

	mov	al, dl
	loop	.begin

	mov	[disp_pos], edi
	
	pop	edi
	pop	edx
	pop	ecx

	ret

disp_int:
	mov	eax, [esp + 4]
	shr	eax, 24
	call	disp_al

	mov	eax, [esp + 4]
	shr	eax, 16
	call	disp_al

	mov	eax, [esp + 4]
	shr	eax, 8
	call	disp_al

	mov	eax, [esp + 4]
	call	disp_al

	mov	ah, 07h
	mov	al, 'h'
	push	edi
	mov	edi, [disp_pos]
	mov	[gs : edi], ax
	add	edi, 4
	mov	[disp_pos], edi
	pop	edi

	ret

disp_str:
	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi

	mov	esi, [ebp + 8]
	mov	edi, [disp_pos]
	mov	ah, 0Fh
.1:
	lodsb
	test	al, al
	jz	.2
	cmp	al, 0Ah
	jnz	.3
	push	eax
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	eax
	jmp	.1
.3:
	mov	[gs : edi], eax
	add	edi, 2
	jmp	.1
.2:
	mov	[disp_pos], edi

	pop	edi
	pop	esi
	pop	ebx
	pop	ebp
	ret

disp_return:
	push	str_return
	call	disp_str
	add	esp, 4

	ret

memcpy:
	push	ebp
	mov	ebp, esp

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]
	mov	esi, [ebp + 12]
	mov	ecx, [ebp + 16]
.1:
	cmp	ecx, 0
	jz	.2

	mov	al, [ds : esi]
	inc	esi

	mov	byte [es : edi], al
	inc	edi

	dec	ecx
	jmp	.1
.2:
	mov	eax, [ebp + 8]

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp

	ret
