
#pragma once

#include "../lib/def.c"

// all things graphical

// text mode stuff

#define __VGA_WIDTH__ 80
#define __VGA_HEIGHT__ 25

#define __BLACK__ 0x0
#define __BLUE__ 0x1
#define __GREEN__ 0x2
#define __CYAN__ 0x3
#define __RED__ 0x4
#define __MAGENTA__ 0x5
#define __BROWN__ 0x6
#define __GREY_LIGHT__ 0x7
#define __GREY_DARK__ 0x8
#define __BLUE_LIGHT__ 0x9
#define __GREEN_LIGHT__ 0xA
#define __CYAN_LIGHT__ 0xB
#define __RED_LIGHT__ 0xC
#define __MAGENTA_LIGHT__ 0xD
#define __BROWN_LIGHT__ 0xE
#define __WHITE__ 0xF

uint16_t vgaCoordinatePlanToLine(uint16_t x, uint16_t y){
  return 80*y+x;
}

void kprintc(uint16_t character, uint16_t pos, uint16_t fg_colour, uint16_t bg_colour){
  uint16_t* buffer=(uint16_t*)0xB8000+pos;
  *buffer=character|(bg_colour<<12)|((fg_colour&0x0F)<<8);
  return;
}

void kprint(uint8_t* string, uint16_t pos, uint16_t fg_colour, uint16_t bg_colour){
  for(uint32_t i=0;string[i]!='\0';i++){
    kprintc(((uint16_t)string[i])|(0<<8), pos+i, fg_colour, bg_colour);
  }
  return;
}

