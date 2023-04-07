	org	0x7C00
	bits	16

	mov	[driveNumber],	dl

	stage1Entry	equ	0x8200

	mov	sp,	0x7C00
	mov	sp,	bp

;	mov	ah,	0x00
;	mov	al,	0x03
;	int	0x10

	call	realModeDiskLoadCHS

    ;mov     bx,     _bDebugMsgDiskLoadSuccess
    ;call    bRealModePrintString0Eh

	jmp	sectorsLoaded

realModeDiskLoadCHS:
	mov	ah,	2 ; BIOS  
	mov	al, 64	; number of sectors to load
	mov	ch,	0 ; Cylinder 0
	mov	cl,	2 ; Sector 0
	mov	dh,	0 ; Head 0
	mov	bx,	0x7E00 ; where to load
	int	13h
	jc	.error 
	ret
	.error: 
	mov	bx,	alertMsgDiskLoadFailure
	call	realModePrintString0Eh
	hlt

realModePrintString0Eh:
	mov	ah,	0Eh
	.while:
	cmp	[bx],	byte	0
	je	.exit
	mov	al,	[bx]
	int	10h
	inc	bx
	jmp	.while
	.exit:
	ret

alertMsgDiskLoadFailure: 
	db	'[!] ERROR : failure while loading sectors. HLT',	0

debugMsgDiskLoadSuccess:
	db	'[D] DEBUG : sectors loaded successfully. ',	0

driveNumber:
	db	0

	times	510 - ($ - $$)	db	0
	dw	0xAA55

sectorsLoaded:
	mov	bx,	debugMsgDiskLoadSuccess
	call	realModePrintString0Eh

	call	enableA20Fast

	cli
	lgdt	[GDTDescriptor]

	mov	eax,	cr0
	or	eax,	0x1
	mov	cr0,	eax

	jmp	codeSegment:protectedModeStart

enableA20Fast:
	in	al,	0x92
	or	al,	0x2
	out	0x92,	al
	ret

GDTNull:
	dd	0x0
	dd	0x0

GDTCode:
	dw	0xFFFF
	dw	0x0
	db	0x0
	db	10011010b
	db	11001111b
	db	0x0

GDTData:
	dw	0xFFFF
	dw	0x0
	db	0x0
	db	10010010b
	db	11001111b
	db	0x0

GDTEnd:

GDTDescriptor: 
	dw	GDTEnd - GDTNull - 1
	dd	GDTNull

codeSegment equ GDTCode - GDTNull
dataSegment equ GDTData - GDTNull

alertMsgProtectedModeFailure:
	db	'[!] ERROR : failure while switching to protected mode. ',	0

	bits	32
protectedModeStart:
	mov	ax,	dataSegment
	mov	ds,	ax
	mov	ss,	ax
	mov	es,	ax
	mov	fs,	ax
	mov	gs,	ax

	mov	ebp,	0x200000
	mov	esp,	ebp

	mov	ebx,	debugMsgProtectedModeStart
	call	protectedModePrintStringVGA

	jmp	codeSegment:stage1Entry	

stage1Entry equ 0x8200

protectedModePrintStringVGA:
	mov	edx,	0xB8000
	.while:
	cmp	[ebx],	byte 0
	je	.exit
	mov	ecx,	[ebx]
	mov	dword[edx],	ecx
	mov	dword[edx + 1],	0x0F
	inc	ebx
	add	edx,	2
	jmp	.while
	.exit:
	ret

debugMsgProtectedModeStart:
	db	'[D] DEBUG : currently in protected mode. ',	0

	times	1024 - ($ - sectorsLoaded)	db	0
