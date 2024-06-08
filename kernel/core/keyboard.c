
#pragma once

#include "../../include/core.h"
#include "graphics.c"
#include "interrupts.c"

#define KEYBOARD_INTERRUPT_VECTOR 33

uint8_t core_keyboard_scancodes_lowercase[84]={0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, ' ', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

uint8_t core_keyboard_scancodes_uppercase[84]={0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, ' ', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0, 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

uint32_t core_keyboard_key_states=0;
/* each bits of core_keyboard_key_states corresponds to a key
bit | key state
0   | escapePressed
1   | controlLeftPressed
2   | shiftLeftPressed
3   | shiftRightPressed
4   | altLeftPressed
5   | capsLockToggled
6   | F1
7   | F2
8   | F3
9   | F4
10  | F5
11  | F6
12  | F7
13  | F8
14  | F9
15  | F10
16  | numberLock
17  | scrollLock
18  | altRightPressed
19  | controlRightPressed
20  | previously 0xE0
*/

uint8_t core_keyboard_buffer;

#define KEYBOARD_SCANCODE_CASE(scancode, position) case scancode:\
core_keyboard_key_states|=(TRUE<<position);\
break;\
case scancode+0x80:\
core_keyboard_key_states&=~(TRUE<<position);\
break

void core_keyboard_handler(){
  __asm__ __volatile__ ("cli");
  uint8_t scancode=core_inb(0x60);
  if(core_keyboard_key_states>>20){
    switch(scancode){
      case 0x48:
        core_keyboard_key_states&=~(TRUE<<20);
        break;
      case 0x4B:
        core_keyboard_key_states&=~(TRUE<<20);
        break;
      case 0x4D:
        core_keyboard_key_states&=~(TRUE<<20);
        break;
      case 0x50:
        core_keyboard_key_states&=~(TRUE<<20);
        break;
      default:
        core_keyboard_key_states&=~(TRUE<<20);
        break;
    }
  }else{
    switch(scancode){
      case 0x0:
        //core_log_str"KEYBOARD ERROR (UNKNOWN SCANCODE)\0", __RED__, __BLUE__);
        return;
      KEYBOARD_SCANCODE_CASE(0x01, 0);
      case 0x0E://just removes everything in the line
        break;
      case 0x0F://tab
        //core_log(' ', __WHITE__, __BLACK__);
        //core_log(' ', __WHITE__, __BLACK__);
        break;
      case 0x1C://enter
        //consoleInput();
        //consoleInit(consoleHeader);
        break;
      KEYBOARD_SCANCODE_CASE(0x1D, 1);
      KEYBOARD_SCANCODE_CASE(0x2A, 2);
      KEYBOARD_SCANCODE_CASE(0x36, 3);
      KEYBOARD_SCANCODE_CASE(0x38, 4);
      case 0x3A://caps lock
        core_keyboard_key_states^=(TRUE<<5);
        break;
      KEYBOARD_SCANCODE_CASE(0x3B, 6);
      KEYBOARD_SCANCODE_CASE(0x3C, 7);
      KEYBOARD_SCANCODE_CASE(0x3D, 8);
      KEYBOARD_SCANCODE_CASE(0x3E, 9);
      KEYBOARD_SCANCODE_CASE(0x3F, 10);
      KEYBOARD_SCANCODE_CASE(0x40, 11);
      KEYBOARD_SCANCODE_CASE(0x41, 12);
      KEYBOARD_SCANCODE_CASE(0x42, 13);
      KEYBOARD_SCANCODE_CASE(0x43, 14);
      KEYBOARD_SCANCODE_CASE(0x44, 15);
      KEYBOARD_SCANCODE_CASE(0x45, 16);
      KEYBOARD_SCANCODE_CASE(0x46, 17);
      KEYBOARD_SCANCODE_CASE(0x57, 18);
      KEYBOARD_SCANCODE_CASE(0x58, 19);
      case 0xE0:
        core_keyboard_key_states|=(TRUE<<20);
        break;
      default:
        if(scancode>=0x80){
          break;
        }
        if((core_keyboard_key_states>>2)||(core_keyboard_key_states>>3)||(core_keyboard_key_states>>5)){
          //core_log(core_keyboard_scancodes_uppercase[scancode], __WHITE__, __BLACK__);
          break;
        }else{
          //core_log(core_keyboard_scancodes_lowercase[scancode], __WHITE__, __BLACK__);
          break;
        }
    }
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

