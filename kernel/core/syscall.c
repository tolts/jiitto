
/*
 * SYSCALLS IN JIITTO
 * arguments: - syscall number in eax
 *            - address to arguments in ecx
 * Use int 80h for syscalls ( in C : __asm__ __volatile__ ("int $0x80") ).
 */

#pragma once

#include "../../include/core.h"
#include "graphics.c"
#include "interrupts.c"

#define SYSCALL_INTERRUPT_VECTOR 0x80

void core_syscall_handler(void){
	__asm__ __volatile__ ("cli");
	uint32_t syscall_number;
	uint32_t syscall_args_address;
	__asm__ __volatile__ ("mov %0, %%eax":"=r"(syscall_number):);
	__asm__ __volatile__ ("mov %0, %%ecx":"=r"(syscall_args_address):);
	switch(syscall_number){
		case 4:
			core_log_str("syscall 4\n\0", core_cursor_position, WHITE, BLACK);
			break;
		default:
			core_log_str("SYSCALL ERROR unknown syscall\n\0", core_cursor_position, WHITE, BLACK);
			break;
	}
	__asm__ __volatile__ ("sti");
	return;
}

void core_syscall_init(void){
#ifdef LOG
	core_log_str("[!] initializing syscalls\n\0", core_cursor_position, WHITE, BLACK);
#endif
	core_idt_set_segment_descriptor(SYSCALL_INTERRUPT_VECTOR, core_syscall_handler, 0x8E);
#ifdef LOG
	core_log_str("[!] syscalls initialized\n\0", core_cursor_position, WHITE, BLACK);
#endif
	return;
}

