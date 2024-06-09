
#define LOG // define to have all logs

#include "../include/core.h"

// general definitions

uint16_t *core_cursor_position;

#include "core/graphics.c"
#include "core/interrupts.c"
#include "core/keyboard.c"

void core_main(void){

  *core_cursor_position=0;

  core_idt_init();
  core_keyboard_init();

  core_log_str("Hello world!\0", core_cursor_position, WHITE, BLACK);
  core_log_str("Again, \nhello!\0", core_cursor_position, WHITE, BLACK);
}

