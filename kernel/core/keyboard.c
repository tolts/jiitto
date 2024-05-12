
#pragma once

#include "../../lib/def.c"
#include "graphics.c"
#include "interrupts.c"
#include "cursor.c"
#include "terminal.c"

#define keyboardInterruptVector 33

uint8_t keyboardScancodesLowercase[84]={0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, ' ', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

uint8_t keyboardScancodesUppercase[84]={0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, ' ', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0, 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

uint32_t keyboardKeyStates=0;
/* each bits of keyboardKeyStates corresponds to a key
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

#define keyboardScancodeCase(scancode, position) case scancode:\
keyboardKeyStates|=(__TRUE__<<position);\
break;\
case scancode+0x80:\
keyboardKeyStates&=~(__TRUE__<<position);\
break

void keyboardHandler(){
  __asm__ __volatile__ ("cli");
  uint8_t scancode=inb(0x60);
  if(keyboardKeyStates>>20){
    switch(scancode){
      case 0x48:
      //  cursorMove(cursorPosition-=80);
        keyboardKeyStates&=~(__TRUE__<<20);
        break;
      case 0x4B:
        cursorMove(--cursorPosition);
        keyboardKeyStates&=~(__TRUE__<<20);
        break;
      case 0x4D:
        if(!(terminalBufferEndPoint>=cursorPosition+1)){
          keyboardKeyStates&=~(__TRUE__<<20);
          break;
        }
        cursorMove(++cursorPosition);
        keyboardKeyStates&=~(__TRUE__<<20);
        break;
      case 0x50:
      //  cursorMove(cursorPosition+=80);
        keyboardKeyStates&=~(__TRUE__<<20);
        break;
      default:
        keyboardKeyStates&=~(__TRUE__<<20);
        break;
    }
  }else{
    switch(scancode){
      case 0x0:
        kprint("KEYBOARD ERROR (UNKNOWN SCANCODE)\0", cursorPosition, __RED__, __BLUE__);
        return;
      keyboardScancodeCase(0x01, 0);
      case 0x0E://backspace
        cursorMove(--cursorPosition);
        kprintc('\0', cursorPosition--, __WHITE__, __BLACK__);
        break;
      case 0x0F://tab
        kprintc(' ', cursorPosition, __WHITE__, __BLACK__);
        kprintc(' ', cursorPosition, __WHITE__, __BLACK__);
        break;
      case 0x1C://enter
        terminalInput(terminalBufferStartPoint, terminalBufferEndPoint);
        cursorMove(cursorPosition+=__VGA_WIDTH__-(cursorPosition%__VGA_WIDTH__));
        terminalInit(terminalHeader);
        break;
      keyboardScancodeCase(0x1D, 1);
      keyboardScancodeCase(0x2A, 2);
      keyboardScancodeCase(0x36, 3);
      keyboardScancodeCase(0x38, 4);
      case 0x3A://caps lock
        keyboardKeyStates^=(__TRUE__<<5);
        break;
      keyboardScancodeCase(0x3B, 6);
      keyboardScancodeCase(0x3C, 7);
      keyboardScancodeCase(0x3D, 8);
      keyboardScancodeCase(0x3E, 9);
      keyboardScancodeCase(0x3F, 10);
      keyboardScancodeCase(0x40, 11);
      keyboardScancodeCase(0x41, 12);
      keyboardScancodeCase(0x42, 13);
      keyboardScancodeCase(0x43, 14);
      keyboardScancodeCase(0x44, 15);
      keyboardScancodeCase(0x45, 16);
      keyboardScancodeCase(0x46, 17);
      keyboardScancodeCase(0x57, 18);
      keyboardScancodeCase(0x58, 19);
      case 0xE0:
        keyboardKeyStates|=(__TRUE__<<20);
        break;
      default:
        if(scancode>=0x80){
          break;
        }
        if((keyboardKeyStates>>2)||(keyboardKeyStates>>3)||(keyboardKeyStates>>5)){
          kprintc(keyboardScancodesUppercase[scancode], cursorPosition, __WHITE__, __BLACK__);
          break;
        }else{
          kprintc(keyboardScancodesLowercase[scancode], cursorPosition, __WHITE__, __BLACK__);
          break;
        }
    }
  }
  picEOI(1);
  __asm__ __volatile__ ("sti");
  return;
}

void keyboardInit(
#ifdef __LOG__
uint8_t* message
#endif
){
  idtSetSegmentDescriptor(keyboardInterruptVector, keyboardHandler, 0x8E);
//  picClearMask(keyboardInterruptVector-32);
#ifdef __LOG__
  kprint(message, 0, __WHITE__, __BLACK__);
#endif
  return;
}

