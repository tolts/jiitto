
#pragma once

#include "../lib/def.c"
#include "graphics.c"
#include "interrupts.c"

#define keyboardInterruptVector 33

uint8_t x=0;

void keyboardHandler(){
  __asm__ __volatile__ ("cli");
  kprintc((uint16_t)inb(0x60), x++, __WHITE__, __BLACK__);
  picEOI(1);
  __asm__ __volatile__ ("sti");
  return;
}

void keyboardInit(uint8_t* message){
  idtSetSegmentDescriptor(keyboardInterruptVector, keyboardHandler, 0x8E);
//  picClearMask(keyboardInterruptVector-32);
  kprint(message, 0, __GREEN_LIGHT__, __GREY_LIGHT__);
  return;
}

