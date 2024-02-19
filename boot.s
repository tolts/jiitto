
section .boot

global START

bits 16
;org 0x7C00

; bootsector

; 1) load bootloader and else
BOOT_SECTOR_LOAD_COUNT equ 2
BOOT_SECTOR_LOAD_CYLINDER equ 0
BOOT_SECTOR_LOAD_HEAD equ 0
BOOT_SECTOR_LOAD_SECTOR equ 2
BOOT_SECTOR_LOAD_BUFFER equ 0x7E00
BOOT_SECTOR_LOAD_DRIVE_NUMBER_FLOPPY equ 0
BOOT_SECTOR_LOAD_DRIVE_NUMBER_HARD_DISK equ 0x80

START:
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

KERNEL_INIT:
  ; fast A20 line switch
  in al, 0x92
  or al, 2
  out 0x02, al

  ; switch to 32-bit protected mode
  cli
  lgdt [GDT_DESCRIPTOR]
  mov eax, cr0
  or eax, 1
  mov cr0, eax

  jmp GDT_CODE_SEGMENT:KERNEL_PROTECTEDMODE_INIT

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

GDT_CODE_SEGMENT equ GDT_CODE_SEG-GDT_NULL_SEG
GDT_DATA_SEGMENT equ GDT_DATA_SEG-GDT_NULL_SEG

KERNEL_INIT_SUCCESS:
  db 'END OF BOOTSECTOR & ACCESS TO KERNEL', 0

bits 32

;extern kernel_main

KERNEL_PROTECTEDMODE_INIT:
  mov ax, GDT_DATA_SEGMENT
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebp, 0x9FC00
  mov esp, ebp

  ;jmp GDT_CODE_SEGMENT:kernel_main

  hlt

times 1024-($-KERNEL_INIT) db 0

