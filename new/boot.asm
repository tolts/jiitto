
org 0x7C00
bits 16

;initEntry equ 0x8200

mov [bootDiskNumber], dl

mov bp, 0x8000
mov sp, bp


mov ah, 0x0
mov al, 0x3
int 0x10

mov ah, 0xB
mov bh, 0x0
mov bl, 0x1
int 0x10



mov bx, bootMsgSuccess
call bootPrint

call bootDiskLoad

jmp $

;jmp bootDiskLoadOk

bootDiskLoad:
    mov ah, 2
    mov al, 2
    mov ch, 0
    mov cl, 2
    mov dh, 0

    mov dl, [bootDiskNumber]
    
    mov bx, 0x7E00
    int 0x13
    jc .fail
    ret
    .fail:
        mov bx, bootMsgDiskFail
        call bootPrint
        hlt

bootPrint:
    mov ah, 0xE
    .while:
        cmp [bx], byte 0
        je .break
        mov al, [bx]
        int 0x10
        inc bx
        jmp .while
    .break:
        ret    

bootMsgSuccess: db 'cc', 0
bootMsgDiskFail: db 'bootDiskFail', 0

bootDiskNumber: db 0

times 510-($-$$) db 0
dw 0xAA55

bootDiskLoadOk:
    mov bx, bootMsgDiskLoadOk
    call bootPrint
    hlt

;TODO memory detection

    call bootA20Enable

    cli
    lgdt [bootGDTD]

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp bootCS:bootPMOk

bootA20Enable:
    in al, 0x92
    or al, 0x2
    out 0x92, al
    ret

bootMsgDiskLoadOk: db 'bootDiskLoadOk', 0

bootGDTN:
    dd 0x0
    dd 0x0

bootGDTCS:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

bootGDTDS:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

bootGDTEnd:

bootGDTD:
    dw bootGDTEnd - bootGDTN - 1
    dd bootGDTN

bootCS equ bootGDTCS - bootGDTN
bootDS equ bootGDTDS - bootGDTN

bits 32

bootPMOk:
    mov ax, bootDS
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x200000
    mov esp, ebp

    mov ebx, bootMsgPMOk
    call bootPrintPM

    jmp $
    ;jmp bootCS:initEntry

bootPrintPM:
    mov edx, 0xB8000
    .while:
        cmp [ebx], byte 0
        je .break
        mov ecx, [ebx]
        mov dword[edx], ecx
        mov dword[edx + 1], 0x0F
        inc ebx
        add edx, 2
        jmp .while
    .break:
        ret

bootMsgPMOk: db 'bootPMOk', 0

times 1024-($-$$) db 0