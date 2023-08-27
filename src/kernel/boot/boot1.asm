;%define DEBUG

org 0x7C00
bits 16

mov bp, 0x8000
mov sp, bp

mov ah, 0x0
mov al, 0x3
int 0x10

mov ah, 0xB
mov bh, 0x0
mov bl, 0x1
int 0x10

mov [bootDiskNumber], dl

;%ifdef DEBUG
mov bx, bootMsgSuccess
jmp bootPrint
;%endif

call bootDiskLoad

jmp bootDiskLoadOk

bootDiskLoad:
    mov ah, 2
    mov al, 64
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
        jmp bootPrint
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

bootMsgSuccess: db "cc", 0
bootMsgDiskFail: db "bootDiskFail", 0

bootDiskNumber: db 0

times 510-($-$$) db 0
dw 0xAA55
