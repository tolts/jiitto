
	bits	32

	global	setIdt
	extern	isrHandler
	extern	irqHandler

	global	isr0
	global	isr1
	global	isr2
	global	isr3
	global	isr4
	global	isr5
	global	isr6
	global	isr7
	global	isr8
	global	isr9
	global	isr10
	global	isr11
	global	isr12
	global	isr13
	global	isr14
	global	isr15
	global	isr16
	global	isr17
	global	isr18
	global	isr19
	global	isr20
	global	isr21
	global	isr22
	global	isr23
	global	isr24
	global	isr25
	global	isr26
	global	isr27
	global	isr28
	global	isr29
	global	isr30
	global	isr31

	global	irq0
	global	irq1
	global	irq2
	global	irq3
	global	irq4
	global	irq5
	global	irq6
	global	irq7
	global	irq8
	global	irq9
	global	irq10
	global	irq11
	global	irq12
	global	irq13
	global	irq14
	global	irq15

setIdt:
	cli
	lidt [eax]
	sti
	ret

isrPreHandling:
	pusha
	mov	ax,	ds
	push	eax
	mov	ax,	0x10
	mov	ds,	ax
	mov	es,	ax
	mov	fs,	ax
	mov	gs,	ax

	call	isrHandler

	pop	eax
	mov	ds,	ax
	mov	es,	ax
	mov	fs,	ax
	mov	gs,	ax
	popa
	add	esp,	8

	sti
	iret

irqPreHandling:
	pusha
	mov	ax,	ds
	push	eax
	mov	ax, 0x10
	mov ds,	ax
	mov es,	ax
	mov	fs,	ax
	mov	gs,	ax

	call	irqHandler

	pop	ebx
	mov	ds,	bx
	mov	es,	bx
	mov	fs,	bx
	mov	gs,	bx
	popa
	add	esp,	8

	sti
	iret

isr0:
	cli
	push	byte	0
	push	byte	0
	jmp	isrPreHandling

isr1:
	cli
	push	byte	0
	push	byte	1
	jmp	isrPreHandling

isr2:
	cli
	push	byte	0
	push	byte	2
	jmp	isrPreHandling

isr3:
	cli
	push	byte	0
	push	byte	3
	jmp	isrPreHandling

isr4:
	cli
	push	byte	0
	push	byte	4
	jmp	isrPreHandling

isr5:
	cli
	push	byte	0
	push	byte	5
	jmp	isrPreHandling

isr6:
	cli
	push	byte	0
	push	byte	6
	jmp	isrPreHandling

isr7:
	cli
	push	byte	0
	push	byte	7
	jmp	isrPreHandling

isr8:
	cli
	push	byte	8
	jmp	isrPreHandling

isr9:
	cli
	push	byte	0
	push	byte	9
	jmp	isrPreHandling

isr10:
	cli
	push	byte	10
	jmp	isrPreHandling

isr11:
	cli
	push	byte	11
	jmp	isrPreHandling

isr12:
	cli
	push	byte	12
	jmp	isrPreHandling

isr13:
	cli
	push	byte	13
	jmp	isrPreHandling

isr14:
	cli
	push	byte	14
	jmp	isrPreHandling

isr15:
	cli
	push	byte	0
	push	byte	15
	jmp	isrPreHandling

isr16:
	cli
	push	byte	0
	push	byte	16
	jmp	isrPreHandling

isr17:
	cli
	push	byte	0
	push	byte	17
	jmp	isrPreHandling

isr18:
	cli
	push	byte	0
	push	byte	18
	jmp	isrPreHandling

isr19:
	cli
	push	byte	0
	push	byte	19
	jmp	isrPreHandling

isr20:
	cli
	push	byte	0
	push	byte	20
	jmp	isrPreHandling

isr21:
	cli
	push	byte	0
	push	byte	21
	jmp	isrPreHandling

isr22:
	cli
	push	byte	0
	push	byte	22
	jmp	isrPreHandling

isr23:
	cli
	push	byte	0
	push	byte	23
	jmp	isrPreHandling

isr24:
	cli
	push	byte	0
	push	byte	24
	jmp	isrPreHandling

isr25:
	cli
	push	byte	0
	push	byte	25
	jmp	isrPreHandling

isr26:
	cli
	push	byte	0
	push	byte	26
	jmp	isrPreHandling

isr27:
	cli
	push	byte	0
	push	byte	27
	jmp	isrPreHandling

isr28:
	cli
	push	byte	0
	push	byte	28
	jmp	isrPreHandling

isr29:
	cli
	push	byte	0
	push	byte	29
	jmp	isrPreHandling

isr30:
	cli
	push	byte	0
	push	byte	30
	jmp	isrPreHandling

isr31:
	cli
	push	byte	0
	push	byte	31
	jmp	isrPreHandling

irq0:
	cli
	push	byte	0
	push	byte	32
	jmp	irqPreHandling

irq1:
	cli
	push	byte	1
	push	byte	33
	jmp irqPreHandling

irq2:
	cli
	push	byte	2
	push	byte	34
	jmp	irqPreHandling

irq3:
	cli
	push	byte	3
	push	byte	35
	jmp	irqPreHandling

irq4:
	cli
	push	byte	4
	push	byte	36
	jmp	irqPreHandling

irq5:
	cli
	push	byte	5
	push	byte	37
	jmp	irqPreHandling

irq6:
	cli
	push	byte	6
	push	byte	38
	jmp	irqPreHandling

irq7:
	cli
	push	byte	7
	push	byte	39
	jmp	irqPreHandling

irq8:
	cli
	push	byte	8
	push	byte	40
	jmp	irqPreHandling

irq9:
	cli
	push	byte	9
	push	byte	41
	jmp	irqPreHandling

irq10:
	cli
	push	byte	10
	push	byte	42
	jmp	irqPreHandling

irq11:
	cli
	push	byte	11
	push	byte	43
	jmp	irqPreHandling

irq12:
	cli
	push	byte	12
	push	byte	44
	jmp	irqPreHandling

irq13:
	cli
	push	byte	13
	push	byte	45
	jmp	irqPreHandling

irq14:
	cli
	push	byte	14
	push	byte	46
	jmp	irqPreHandling

irq15:
	cli
	push	byte	15
	push	byte	47
	jmp	irqPreHandling
