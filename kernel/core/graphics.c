
#pragma once

#include "../../include/core.h"

// all things graphical

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define MAGENTA 0x5
#define BROWN 0x6
#define GREY_LIGHT 0x7
#define GREY_DARK 0x8
#define BLUE_LIGHT 0x9
#define GREEN_LIGHT 0xA
#define CYAN_LIGHT 0xB
#define RED_LIGHT 0xC
#define MAGENTA_LIGHT 0xD
#define BROWN_LIGHT 0xE
#define WHITE 0xF

void core_log(uint8_t character, uint16_t pos, uint16_t fg_colour, uint16_t bg_colour){
  // different from printf as core_log moves the cursor
  uint16_t *buffer=(uint16_t*)0xB8000+pos;
  *buffer=(uint16_t)character|(bg_colour<<12)|((fg_colour&0x0F)<<8);
  return;
}

void core_log_str(uint8_t *string, uint16_t *pos, uint16_t fg_colour, uint16_t bg_colour){
  for(uint32_t i=0;string[i];i++){
    while(*pos>=2000){
      uint16_t *buf=(uint16_t*)0xB8000;
      for(uint16_t i=0; i<2000; i++){
        buf[i]=buf[i+80];
      }
      *pos-=80;
    }
    if(string[i]==10){
      *pos+=VGA_WIDTH-((*pos)%VGA_WIDTH);
    }else{
      core_log(string[i], (*pos)++, fg_colour, bg_colour);
    }
  }
  return;
}

