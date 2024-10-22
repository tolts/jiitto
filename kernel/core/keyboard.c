
#pragma once

#include "../../include/core.h"
#include "graphics.c"
#include "interrupts.c"

#define KEYBOARD_IRQ_VECTOR 1

struct core_keyboard_key_states_t{
  uint64_t first_64_bits;
  uint64_t last_64_bits;
} __attribute__((packed)) core_keyboard_key_states;

void core_keyboard_handler(){
  __asm__ __volatile__ ("cli");
  uint8_t scancode=core_inb(0x60);
  //core_log(scancode, *core_cursor_position, WHITE, BLACK); // to see scancode ascii equivalents
  if(scancode>=0x81){
    scancode-=0x80;
  }
  if(scancode<=63){
    core_keyboard_key_states.first_64_bits^=(1<<scancode);
  }else{
    core_keyboard_key_states.last_64_bits^=(1<<(scancode-64));
  }
  core_pic_eoi(KEYBOARD_IRQ_VECTOR);
  __asm__ __volatile__ ("sti");
  return;
}

void core_keyboard_init(void){
#ifdef LOG
  core_log_str("[!] initializing keyboard\n\0", core_cursor_position, WHITE, BLACK);
#endif
  core_idt_set_segment_descriptor(KEYBOARD_IRQ_VECTOR+32, core_keyboard_handler, 0x8E);
#ifdef LOG
  core_log_str("[!] keyboard initialized\n\0", core_cursor_position, WHITE, BLACK);
#endif
  return;
}

