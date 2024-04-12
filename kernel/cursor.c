
#pragma once

#include "../lib/def.c"
#include "graphics.c"

uint16_t cursorPosition;

uint16_t cursorGetPosition(void){
  outb(0x3D4, 0x0F);
  uint16_t position=(uint16_t)inb(0x3D5);
  outb(0x3D4, 0x0E);
  return (((uint16_t)inb(0x3D5))<<8)|position;
}

void cursorEnable(uint8_t cursorStart, uint8_t cursorEnd){
  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5)&0xC0)|cursorStart);
  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3D5)&0xE0)|cursorEnd);
  return;
}

void cursorDisable(void){
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);
  return;
}

void cursorMove(uint16_t position){
  while(position>=2000){
    uint16_t* buffer=(uint16_t*)0xB8000;
    for(uint16_t i=0; i<2000; i++){
      buffer[i]=buffer[i+80];
    }
    position-=80;
  }
  cursorPosition=position;
  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(position&0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)((position>>8)&0xFF));
  return;
}

