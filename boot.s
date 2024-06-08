
section .boot

global START

bits 16

; bootsector

; load bootloader and else
BOOT_SECTOR_LOAD_COUNT equ 34; HERE MAYBE LAYS YOUR PROBLEM
BOOT_SECTOR_LOAD_CYLINDER equ 0
BOOT_SECTOR_LOAD_HEAD equ 0
BOOT_SECTOR_LOAD_SECTOR equ 2
BOOT_SECTOR_LOAD_BUFFER equ 0x7E00
BOOT_SECTOR_LOAD_DRIVE_NUMBER_FLOPPY equ 0
BOOT_SECTOR_LOAD_DRIVE_NUMBER_HARD_DISK equ 0x80

START:
  mov ah, 0
  mov al, 3
  int 0x10

  mov ah, 2
  mov al, BOOT_SECTOR_LOAD_COUNT
  mov ch, BOOT_SECTOR_LOAD_CYLINDER&0xFF
  mov dh, BOOT_SECTOR_LOAD_HEAD
  mov cl, BOOT_SECTOR_LOAD_SECTOR|((BOOT_SECTOR_LOAD_CYLINDER>>2)&0xC0)
  mov bx, BOOT_SECTOR_LOAD_BUFFER
;mov dl, BOOT_SECTOR_LOAD_DRIVE_NUMBER_FLOPPY
; the above is unnecessary for the BIOS load the drive number by default
  int 0x13
  jc BOOT_SECTOR_DISK_LOAD_FAIL
  jnc KERNEL_INIT

BOOT_SECTOR_DISK_LOAD_FAIL:
  mov bx, BOOT_SECTOR_DISK_LOAD_FAIL_MSG
  mov ah, 0x0E
  .HANG:
    cmp [bx], byte 0
    je .EXIT
    mov al, [bx]
    int 0x10
    inc bx
    jmp .HANG
  .EXIT:
    hlt

BOOT_SECTOR_DISK_LOAD_FAIL_MSG:
  db 'HALTED FOR DISK LOAD FAIL', 0

BOOT_SECTOR_SIZE equ $-$$
%if BOOT_SECTOR_SIZE>510
  %error "bootsector: code is too large for the bootsector (<510)"
%endif
times 510-($-$$) db 0
dw 0xAA55

; bootloader (bootsector was previous sector)

; switch to 32-bit protected mode
KERNEL_INIT:
  ; fast A20 line switch
  in al, 0x92
  or al, 2
  out 0x02, al

  ; switch to pm mode
  cli
  lgdt [GDT_DESCRIPTOR]
  mov eax, cr0
  or eax, 1
  mov cr0, eax

  jmp GDT_CODE_SEGMENT:KERNEL_PROTECTEDMODE_INIT

; gdt
GDT_NULL_SEG:
  dq 0

GDT_CODE_SEG:
  dw 0xFFFF
  dw 0
  db 0
  db 0x9A
  db 0xCF
  db 0

GDT_DATA_SEG:
  dw 0xFFFF
  dw 0
  db 0
  db 0x92
  db 0xCF
  db 0

GDT_DESCRIPTOR:
  dw GDT_DESCRIPTOR-GDT_NULL_SEG-1
  dd GDT_NULL_SEG

global GDT_CODE_SEGMENT
global GDT_DATA_SEGMENT

GDT_CODE_SEGMENT equ GDT_CODE_SEG-GDT_NULL_SEG
GDT_DATA_SEGMENT equ GDT_DATA_SEG-GDT_NULL_SEG

KERNEL_INIT_SUCCESS:
  db 'END OF BOOTSECTOR & ACCESS TO KERNEL', 0

bits 32

extern core_main

; enter kernel
KERNEL_PROTECTEDMODE_INIT:
  mov ax, GDT_DATA_SEGMENT
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

; set the stack (grows downwards)
  mov ebp, 0x9FC00
  mov esp, ebp

  call GDT_CODE_SEGMENT:core_main

; main loop is here
  jmp $

times 512-($-KERNEL_INIT) db 0

; ISRs for interrupts (also IRQs)

section .text

global core_isr_stub_table

core_isr_stub_table:
%assign i 0
%rep 48
  dd ISR_STUB_%+i
%assign i i+1
%endrep

extern core_exception_handler

ISR_COMMON_HANDLER:
  pusha
  mov ax, ds
  push eax
  mov ax, GDT_DATA_SEGMENT
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  call core_exception_handler
  pop eax
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  popa
  add esp, 8
  sti
  iretd

extern core_irq_handler

IRQ_COMMON_HANDLER:
  pusha
  mov ax, ds
  push eax
  mov ax, GDT_DATA_SEGMENT
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  call core_irq_handler
  pop eax
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  popa
  add esp, 8
  sti
  iretd

%macro ISR_STUB_ERROR 1
ISR_STUB_%+%1:
  cli
  push %1
  jmp ISR_COMMON_HANDLER
%endmacro

%macro ISR_STUB_NO_ERROR 1
ISR_STUB_%+%1:
  cli
  push 0
  push %1
  jmp ISR_COMMON_HANDLER
%endmacro

%macro IRQ_STUB 1
ISR_STUB_%+%1:
  cli
  push %1
  push 32+%1
  jmp IRQ_COMMON_HANDLER
%endmacro

ISR_STUB_NO_ERROR 0
ISR_STUB_NO_ERROR 1
ISR_STUB_NO_ERROR 2
ISR_STUB_NO_ERROR 3
ISR_STUB_NO_ERROR 4
ISR_STUB_NO_ERROR 5
ISR_STUB_NO_ERROR 6
ISR_STUB_NO_ERROR 7
ISR_STUB_ERROR 8
ISR_STUB_NO_ERROR 9
ISR_STUB_ERROR 10
ISR_STUB_ERROR 11
ISR_STUB_ERROR 12
ISR_STUB_ERROR 13
ISR_STUB_ERROR 14
ISR_STUB_NO_ERROR 15
ISR_STUB_NO_ERROR 16
ISR_STUB_ERROR 17
ISR_STUB_NO_ERROR 18
ISR_STUB_NO_ERROR 19
ISR_STUB_NO_ERROR 20
ISR_STUB_NO_ERROR 21
ISR_STUB_NO_ERROR 22
ISR_STUB_NO_ERROR 23
ISR_STUB_NO_ERROR 24
ISR_STUB_NO_ERROR 25
ISR_STUB_NO_ERROR 26
ISR_STUB_NO_ERROR 27
ISR_STUB_NO_ERROR 28
ISR_STUB_NO_ERROR 29
ISR_STUB_ERROR 30
ISR_STUB_NO_ERROR 31

IRQ_STUB 32
IRQ_STUB 33
IRQ_STUB 34
IRQ_STUB 35
IRQ_STUB 36
IRQ_STUB 37
IRQ_STUB 38
IRQ_STUB 39
IRQ_STUB 40
IRQ_STUB 41
IRQ_STUB 42
IRQ_STUB 43
IRQ_STUB 44
IRQ_STUB 45
IRQ_STUB 46
IRQ_STUB 47

