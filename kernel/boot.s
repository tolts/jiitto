
section .boot

global START

bits 16

; bootsector

; load bootloader and else
boot_sector_load_count equ 34; HERE MAYBE LAYS YOUR PROBLEM
boot_sector_load_cylinder equ 0
boot_sector_load_head equ 0
boot_sector_load_sector equ 2
boot_sector_load_buffer equ 0x7E00

; FAT32 compatibility
global BS_jmpBoot

BS_jmpBoot:
  jmp short boot_sector_main
BS_OEMName: db ' JIITTO '
BPB_BytsPerSec: dw 512
BPB_SecPerClus: db 1
BPB_RsvdSecCnt: dw 32
BPB_NumFATs: db 2
BPB_RootEntCnt: dw 0
BPB_TotSec16: dw 0
BPB_Media: db 0xF0
BPB_FATSz16: dw 0
BPB_SecPerTrk: dw 18
BPB_NumHeads: dw 2
BPB_HiddSec: dd 0
BPB_TotSec32: dd 0

BPB_FATSz32: dd 0
BPB_ExtFlags: dw 0
BPB_FSVer: dw 0
BPB_RootClus: dd 2
BPB_FSInfo: dw 1
BPB_BkBootSec: dw 6
BPB_Reserved: times 12 db 0
BS_DrvNum: db 0
BS_Reserved1: db 0
BS_BootSig: db 0x29
BS_VolID: dd 0
BS_VolLab: db ' JIITTO OS '
BS_FilSysType: db 'FAT32   '



boot_sector_main:
  mov ah, 0
  mov al, 3
  int 0x10

  mov ah, 0x01
  mov ch, 0x3F
  int 0x10

  mov ah, 2
  mov al, boot_sector_load_count
  mov ch, boot_sector_load_cylinder&0xFF
  mov dh, boot_sector_load_head
  mov cl, boot_sector_load_sector|((boot_sector_load_cylinder>>2)&0xC0)
  mov bx, boot_sector_load_buffer
  int 0x13
  jc boot_sector_disk_load_fail
  jnc kernel_init

boot_sector_disk_load_fail:
  mov bx, boot_sector_disk_load_fail_msg
  mov ah, 0x0E
  .hang:
    cmp [bx], byte 0
    je .exit
    mov al, [bx]
    int 0x10
    inc bx
    jmp .hang
  .exit:
    hlt

boot_sector_disk_load_fail_msg:
  db 'HALTED FOR DISK LOAD FAIL', 0

boot_sector_size equ $-$$
%if boot_sector_size>510
  %error "bootsector: code is too large for the bootsector (<510)"
%endif
times 510-($-$$) db 0
dw 0xAA55

; bootloader (bootsector was previous sector)

; switch to 32-bit protected mode
kernel_init:
  ; fast A20 line switch
  in al, 0x92
  or al, 2
  out 0x02, al

  ; switch to pm mode
  cli
  lgdt [gdt_descriptor]
  mov eax, cr0
  or eax, 1
  mov cr0, eax

  jmp gdt_code_segment:kernel_protectedmode_init

; gdt
gdt_null_seg:
  dq 0

gdt_code_seg:
  dw 0xFFFF
  dw 0
  db 0
  db 0x9A
  db 0xCF
  db 0

gdt_data_seg:
  dw 0xFFFF
  dw 0
  db 0
  db 0x92
  db 0xCF
  db 0

gdt_descriptor:
  dw gdt_descriptor-gdt_null_seg-1
  dd gdt_null_seg

gdt_code_segment equ gdt_code_seg-gdt_null_seg
gdt_data_segment equ gdt_data_seg-gdt_null_seg

bits 32

extern core_main
;extern sys_main

; enter kernel
kernel_protectedmode_init:
  mov ax, gdt_data_segment
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

; set the stack (grows downwards)
  mov ebp, 0x9FC00
  mov esp, ebp

  call gdt_code_segment:core_main
;  call gdt_code_segment:sys_main

  jmp $

times 512-($-kernel_init) db 0

; everything connected to kernel.c and the likes

section .text

; ISRs for interrupts (also IRQs)

global core_isr_stub_table

core_isr_stub_table:
%assign i 0
%rep 48
  dd isr_stub_%+i
%assign i i+1
%endrep

extern core_exception_handler

isr_common_handler:
  pusha
  mov ax, ds
  push eax
  mov ax, gdt_data_segment
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

irq_common_handler:
  pusha
  mov ax, ds
  push eax
  mov ax, gdt_data_segment
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

%macro isr_stub_error 1
isr_stub_%+%1:
  cli
  push %1
  jmp isr_common_handler
%endmacro

%macro isr_stub_no_error 1
isr_stub_%+%1:
  cli
  push 0
  push %1
  jmp isr_common_handler
%endmacro

%macro irq_stub 1
isr_stub_%+%1:
  cli
  push %1
  push 32+%1
  jmp irq_common_handler
%endmacro

isr_stub_no_error 0
isr_stub_no_error 1
isr_stub_no_error 2
isr_stub_no_error 3
isr_stub_no_error 4
isr_stub_no_error 5
isr_stub_no_error 6
isr_stub_no_error 7
isr_stub_error 8
isr_stub_no_error 9
isr_stub_error 10
isr_stub_error 11
isr_stub_error 12
isr_stub_error 13
isr_stub_error 14
isr_stub_no_error 15
isr_stub_no_error 16
isr_stub_error 17
isr_stub_no_error 18
isr_stub_no_error 19
isr_stub_no_error 20
isr_stub_no_error 21
isr_stub_no_error 22
isr_stub_no_error 23
isr_stub_no_error 24
isr_stub_no_error 25
isr_stub_no_error 26
isr_stub_no_error 27
isr_stub_no_error 28
isr_stub_no_error 29
isr_stub_error 30
isr_stub_no_error 31

irq_stub 32
irq_stub 33
irq_stub 34
irq_stub 35
irq_stub 36
irq_stub 37
irq_stub 38
irq_stub 39
irq_stub 40
irq_stub 41
irq_stub 42
irq_stub 43
irq_stub 44
irq_stub 45
irq_stub 46
irq_stub 47
