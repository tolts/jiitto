
#define LOG // define to have all logs

#include "include/core.h"

// general definitions

uint16_t *core_cursor_position;

#include "kernel/core/graphics.c"
#include "kernel/core/interrupts.c"
#include "kernel/core/keyboard.c"

void core_main(void){

  *core_cursor_position=3;

  core_idt_init(
#ifdef LOG
    "interrupts enabled\0"
#endif
);
  core_keyboard_init(
#ifdef LOG
    "keyboard enabled\0"
#endif
);

  core_log_str("Hello world!\0", core_cursor_position, WHITE, BLACK);
  core_log_str("Again, hello!\0", core_cursor_position, WHITE, BLACK);
}

