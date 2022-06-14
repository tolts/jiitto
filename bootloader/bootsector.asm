;	bootsector.asm
;   Copyright (C) 2022 schochtlts
;
;   This program is free software: you can redistribute it and/or modify
;   it under the terms of the GNU General Public License as published by
;   the Free Software Foundation, either version 3 of the License, or
;   (at your option) any later version.
;
;   This program is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License
;   along with this program.  If not, see <https://www.gnu.org/licenses/>.

[org 0x7C00]
[bits 16]
mov [boot_drive], dl

mov sp, 0x7E00 ; set the stack
mov sp, bp

mov ah, 0x00 ; set video mode
mov al, 0x03
int 0x10

mov ah, 0x0B ; set color
mov bh, 0x00
mov bl, 0x01 ; 0x01 for blue, 0x02 for green, 0x03 for green-ish light blue, 0x04 for red, ...
int 0x10

mov bx, msg16_1.booted
call real_mode_print_string

call real_mode_disk_load

jmp 0x7E00

real_mode_disk_load: 
    mov ah, 2 ; BIOS function for disk reading
    mov bx, 0x7E00 ; where to load
    mov al, 3 ; number of sectors to load
    mov ch, 0 ; Cylinder 0
    mov dh, 0 ; Head 0
    mov cl, 2 ; Sector 0
    int 0x13
    jc .error
    ret
    .error: 
        mov bx, msg16_1.disk_load_failed
        call real_mode_print_string
        hlt

real_mode_print_string: 
    mov ah, 0x0E
    .hang: 
        cmp [bx], byte 0
        je .exit
        mov al, [bx]
        int 0x10
        inc bx
        jmp .hang
    .exit: 
        ret

msg16_1: 
    .booted: db ' debug : booted from 0000:7C00. ', 0
    .disk_load_failed: db ' error : failed to read disk. halt. ', 0

boot_drive: db 0

times 510-($-$$) db 0
dw 0xAA55