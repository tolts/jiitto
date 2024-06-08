
#pragma once

#include "../../include/core.h"
#include "graphics.c"
#include "interrupts.c"

#define KEYBOARD_INTERRUPT_VECTOR 33

struct core_keyboard_key_states_t{
  uint64_t first_64_bits;
  uint64_t last_64_bits;
} __attribute__((packed)) core_keyboard_key_states;

void core_keyboard_handler(){
  __asm__ __volatile__ ("cli");
  uint8_t scancode=core_inb(0x60);
  core_log(scancode, *core_cursor_position, WHITE, BLACK);
  if(scancode>=0x81){
    scancode-=0x80;
  }
  if(scancode<=63){
    core_keyboard_key_states.first_64_bits^=(1<<scancode);
  }else{
    core_keyboard_key_states.last_64_bits^=(1<<(scancode-64));
  }
  core_pic_eoi(1);
  __asm__ __volatile__ ("sti");
  return;
}

void core_keyboard_init(
#ifdef LOG
  uint8_t* message
#endif
){
  core_idt_set_segment_descriptor(KEYBOARD_INTERRUPT_VECTOR, core_keyboard_handler, 0x8E);
//  picClearMask(keyboardInterruptVector-32);
#ifdef LOG
  core_log_str(message, core_cursor_position, WHITE, BLACK);
#endif
  return;
}

