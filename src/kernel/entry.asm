	bits	32

	extern	init
	global	_start

_start:
	call	init
	jmp	$

%include "int/int.asm"