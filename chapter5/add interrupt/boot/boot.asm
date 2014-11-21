org		0x7c00

BASE_STACK	equ		0x7c00

%include	"load.inc"

		jmp	short LABEL_START
		nop

%include	"fat12hdr.inc"

LABEL_START:
		mov	ax, cs
		mov	ds, ax
		mov	es, ax
		mov	ss, ax
		mov	sp, BASE_STACK  ;将sp指向BASE——STACK是因为sp一直都是指向栈顶，
					;所以，栈是向内存的低位伸展的
								
		;清屏
		mov	ax, 0x600
		mov	bx, 0x700
		mov	cx, 0
		mov	dx, 0x184f
		int	0x10

		mov	dh, 0
		call	puts

		xor	ah, ah
		xor	dl, dl
		int	0x13

		mov	word [cur_sec], FST_SEC_IN_ROOT_DIR
LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
		cmp	word [nr_sec_unread], 0
		jz	LABEL_NO_LOADERBIN
		dec	word [nr_sec_unread]
		mov	ax, BASE_LOADER
		mov	es, ax
		mov	bx, OFFSET_LOADER  ;int 13能将软盘数据读到 es:bx 指向的缓冲区中
		mov	ax, [cur_sec]    ;ax存的是扇区号
		mov	cl, 1    ;cl存的是读取的扇区数
		call	readsec

		mov	si, NAME_LOADER
		mov	di, OFFSET_LOADER
		cld
		mov	dx, 10h    ;每个扇区的条目数
LABEL_SEARCH_FOR_LOADER:
		cmp	dx, 0
		jz	LABEL_GOTO_NEXT_SECTOR
		dec	dx
		add	word [readtimes], 1
		mov	cx, 11    ;"loader  bin"长度为11.
LABEL_CMP_FILENAME:
		cmp	cx, 0
		jz	LABEL_FILENAME_FOUND
		dec	cx
		lodsb		;ds:si->al 
				;ds:si 指向"loader  bin"即正确的文件名
		cmp	al, byte [es:di]	;es:di 指向的是 int 13 读取的文件的缓冲区，即指向当前读取的条目的文件名
		jz	LABEL_GO_ON
		jmp	LABEL_DIFFERENT

LABEL_GO_ON:
		inc	di
		jmp	LABEL_CMP_FILENAME

LABEL_DIFFERENT:
		;当文件名有一个字母不相同时，es:di 则必须指向下一个条目
		and	di, 0xFFE0			;将di的低五位清零，以便使di指向本条目的开头
		add	di, 0x20			;指向下一个条目
		mov	si, NAME_LOADER			;ds:si重新指向正确的文件名的开头
		jmp	LABEL_SEARCH_FOR_LOADER

LABEL_GOTO_NEXT_SECTOR:
		add	word [cur_sec], 1
		jmp	LABEL_SEARCH_IN_ROOT_DIR_BEGIN

LABEL_NO_LOADERBIN:
		mov	dh, 2
		call	puts
		call	print_int
		jmp	$

LABEL_FILENAME_FOUND:
		mov	ax, NR_SEC_IN_ROOTDIR
		and	di, 0xFFE0				;使es:di 指向条目的起始位置
		add	di, 0x1A				;每一个条目偏移 1A 是该条目对应的起始簇号
		mov	cx, word [es:di]
		push	cx					;先将簇号保存起来，后面根据该簇号在fat1找到下一个簇号
		add	cx, ax
		add	cx, ALL_SEC_FROM_FST_TO_FAT2    ;从起始扇区到FAT2结束总共有19个扇区，
							;但是数据区的第一个簇号是2，所以要减去0,1这两个不存在的簇号
		mov	ax, BASE_LOADER
		mov	es, ax
		mov	bx, OFFSET_LOADER
		mov	ax, cx				;这一次是真的要将数据读取并加载到BASE_LOADER:OFFSET_LOADER这段内存了
							;在找loader  bin 的时候是将这个地方当做临时缓冲区
LABEL_GOON_LOADING_FILE:
		;每读入一个扇区就显示一个 . 
		push	ax
		push	bx
		mov	ah, 0x0E
		mov	al, '.'
		mov	bl, 0x0F
		int	0x10
		pop	bx
		pop	ax

		mov	cl, 1
		call	readsec
		pop	ax
		call	get_next_clu
		cmp	ax, 0xFFF
		jz	LABEL_FILE_END
		push	ax				;这一步与上面的push cx 的作用是一样的，都是要保存簇号
		mov	dx, NR_SEC_IN_ROOTDIR
		add	ax, dx
		add	ax, ALL_SEC_FROM_FST_TO_FAT2
		add	bx, [BPB_BytsPerSec]		;es:bx跳到下一个扇区
		jmp	LABEL_GOON_LOADING_FILE

LABEL_FILE_END:
		mov	dh, 1
		call	puts

		jmp	BASE_LOADER:OFFSET_LOADER

;macro
FST_SEC_IN_ROOT_DIR		equ	19
BASE_LOADER			equ	09000h
OFFSET_LOADER			equ	0100h
NR_SEC_IN_ROOTDIR		equ	14
ALL_SEC_FROM_FST_TO_FAT2	equ	19 - 2
SEC_NO_OF_FAT1			equ	1
;visable
readtimes		dw	0
cur_sec			dw	0
is_odd			db	0
NAME_LOADER		db	"LOADER  BIN", 0
nr_sec_unread		dw	14
len_str			equ	9
str_boot		db	"Booting  "
str_ready		db	"ready.   "
str_noloader		db	"No loader"

readsec:
	;在调用这个函数之前，ax被赋予要度的扇区号，cl被赋予要度的扇区数
	;说句题外话，这个函数是十分典型的 stdcall 函数调用,即函数自身清理堆栈
	push	bp
	mov	bp, sp
	sub	esp, 2

	mov	byte [bp - 2], cl	;对应上面的 sub esp, 2 即在堆栈留出两个字节来保存 cl
	push	bx
	mov	bl, [BPB_SecPerTrk]
	div	bl			;al 是商，ah 是余数
	inc	ah
	mov	cl, ah
	mov	dh, al
	shr	al, 1
	mov	ch, al
	and	dh, 1
	pop	bx
	mov	dl, [BS_DrvNum]
.goon_reading:
	mov	ah, 2
	mov	al, byte [bp - 2]
	int	0x13
	jc	.goon_reading

	add	esp, 2			;上面减掉2，这里就增加2
	pop	bp			;恢复堆栈

	ret

get_next_clu:
	push	es
	push	bx
	push	ax
	mov	ax, BASE_LOADER
	sub	ax, 0100h
	mov	es, ax
	pop	ax
	mov	byte [is_odd], 0
	mov	bx, 3
	mul	bx
	mov	bx, 2
	div	bx
	cmp	dx, 0
	jz	LABEL_EVEN
	mov	byte [is_odd], 1
LABEL_EVEN:
	xor	dx, dx
	mov	bx, [BPB_BytsPerSec]
	div	bx
	push	dx
	mov	bx, 0
	add	ax, SEC_NO_OF_FAT1
	mov	cl, 2
	call	readsec

	pop	dx
	add	bx, dx
	mov	ax, [es:bx]
	cmp	byte [is_odd], 1
	jnz	LABEL_EVEN_2
	shr	ax, 4
LABEL_EVEN_2:
	and	ax, 0FFFh

	pop	bx
	pop	es
	ret

puts:
	mov	ax, len_str
	mul	dh
	add	ax, str_boot
	mov	bp, ax
	mov	ax, ds
	mov	es, ax
	mov	cx, len_str
	mov	ax, 01301h
	mov	bx, 0007h
	mov	dl, 0
	int	10h
	ret

print_int:
	mov	ax, [readtimes]
.start:
	cmp	ax, 0
	jz	.finish
	mov	bx, 10
	div	bx
	mov	al, dl
	mov	ah, 0Ch
	mov	[gs:((80 * 10 + 0) * 2)], ax
	jmp	.start
.finish:
	ret

times	510 - ($ - $$)	db	0
dw	0xaa55
