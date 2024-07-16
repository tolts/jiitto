
#define LOG // define to have all logs

#include "../include/core.h"

uint16_t *core_cursor_position;

#include "core/interrupts.c"
#include "core/keyboard.c"
#include "core/syscall.c"
#include "core/cpu.c"

void core_system_detection(void){

	core_log_str("[!] begin system detection\n\0", core_cursor_position, WHITE, BLACK);

	// detect cpu TODO COMPLETE CPU DETECTION SUPPORT
	if(core_cpuid_available()){
		core_cpuid_get_vendor_id();
		core_log_str("CPU vendor ID : '\0", core_cursor_position, WHITE, BLACK);
		core_log_str(core_cpu_vendor_id, core_cursor_position, WHITE, BLACK);
		core_log_str("'\n\0", core_cursor_position, WHITE, BLACK);
	}else{
		core_log_str("CPU vendor ID : (CPUID not available)\0\n", core_cursor_position, WHITE, BLACK);
	}

	// detect memory

	// detect keyboard
	// detect filesystem
	// detect LuaJit
	// detect system/init.lua AND config/init.lua

	core_log_str("[!] end of system detection\n\0", core_cursor_position, WHITE, BLACK);
	return;
}

void core_main(void){
	*core_cursor_position=0;

#ifdef LOG
	core_log_str("[!] initialisation of core components\n\0", core_cursor_position, WHITE, BLACK);
#endif
	core_idt_init();
	core_keyboard_init();
	core_syscall_init();
#ifdef LOG
	core_log_str("[!] core components initialised\n\0", core_cursor_position, WHITE, BLACK);
#endif

	core_system_detection();

//	core_system_initialise();
}

