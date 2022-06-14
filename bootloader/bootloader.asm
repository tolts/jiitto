;	bootloader.asm
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

[org 0x7E00]
[bits 16]
mov bx, msg16_2.disk_load_success
call real_mode_print_string

call enable_A20

cli
lgdt[gdt_descriptor]

mov eax, cr0
or eax, 0x1
mov cr0, eax

jmp code_segment:enabled_protected_mode

enable_A20: 
    in al, 0x92
    or al, 0x2
    out 0x92, al
    ret

gdt_null: 
    dd 0x0
    dd 0x0

gdt_code: 
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

gdt_data: 
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end: 

gdt_descriptor: 
    dw gdt_end - gdt_null - 1
    dq gdt_null

code_segment equ gdt_code - gdt_null
data_segment equ gdt_data - gdt_null

[bits 32]
edit_gdt: 
    mov [gdt_code + 6], byte 10101111b
    mov [gdt_data + 6], byte 10101111b
    ret

[bits 16]
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

msg16_2: 
    .disk_load_success: db ' debug : loaded disk to 0000:7E00. ', 0
    .protected_mode_fail: db ' error : failed to enter 32-bit protected mode. ', 0

[bits 32]
enabled_protected_mode: 
    mov ax, data_segment
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x7F000
    mov esp, ebp

    call protected_mode_clear_screen

    mov ebx, msg32.protected_mode_success
    call protected_mode_print_string

    call detect_cpuid
    call detect_long_mode
    call setup_identity_paging
    call edit_gdt

    jmp code_segment:enabled_long_mode

setup_identity_paging:

	mov edi, 0x1000
	mov cr3, edi

	mov dword [edi], 0x2003
	add edi, 0x1000
	mov dword [edi], 0x3003
	add edi, 0x1000
	mov dword [edi], 0x4003
	add edi, 0x1000

	mov ebx, 0x00000003
	mov ecx, 512

	.set_entry:
		mov dword [edi], ebx
		add ebx, 0x1000
		add edi, 8
		loop .set_entry

	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	ret

detect_cpuid:

	pushfd 
	pop eax

	mov ecx, eax

	xor eax, 1 << 21

	push eax
	popfd

	pushfd 
	pop eax

	push ecx 
	popfd

	xor eax,ecx
	jz .no_CPUid
	ret
    .no_CPUid:
        call protected_mode_clear_screen
        mov ebx, msg32.no_cpuid
        call protected_mode_print_string
        hlt

detect_long_mode:
	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz .no_long_mode
	ret
    .no_long_mode: 
        call protected_mode_clear_screen
        mov ebx, msg32.no_long_mode
        hlt

protected_mode_print_string:
    mov edx, 0xB8000
    .hang: 
        cmp [ebx], byte 0
        je .exit
        mov ecx, [ebx]
        mov dword[edx], ecx
        mov dword[edx + 1], 0x0F
        inc ebx
        add edx, 2
        jmp .hang
    .exit: 
        ret

protected_mode_clear_screen: 
    mov ecx, 0
    mov edx, 0xB8000
    .hang: 
        cmp ecx, 1024
        je .exit
        mov [edx], byte 0
        inc ecx
        inc edx
        jmp .hang
    .exit: 
        ret

msg32: 
    .protected_mode_success: db ' debug : entered 32-bit protected mode. ', 0
    .no_cpuid: db ' error : CPUID not found. halt. ', 0
    .cpuid_available: db ' debug : CPUID is available. ', 0
    .no_long_mode: db ' error : 64-bit long mode not available. halt. ', 0
    .long_mode_available: db ' debug : 64-bit long mode is available. ', 0

[bits 64]
enabled_long_mode:
    mov ebp, 0x7D00
    mov esp, ebp

    call long_mode_clear_screen

    mov rax, msg64.long_mode_success
    call long_mode_print_string

    jmp kernel_entry

long_mode_print_string:
    mov edx, 0xB8000
    .hang: 
        cmp [rax], byte 0
        je .exit
        mov ecx, [rax]
        mov dword[edx], ecx
        mov dword[edx + 1], 0x0F
        inc rax
        add edx, 2
        jmp .hang
    .exit: 
        ret

long_mode_clear_screen: 
    mov ecx, 0
    mov edx, 0xB8000
    .hang: 
        cmp ecx, 1024
        je .exit
        mov [edx], byte 0
        inc ecx
        inc edx
        jmp .hang
    .exit: 
        ret

msg64:
    .long_mode_success: db ' debug : entered 64-bit long mode. ', 0

kernel_entry equ 0x8200

times 1024-($-$$) db 0
