
#pragma once

#include "../misc/types.c"

#define pic1 0x20
#define pic2 0xA0
#define pic1Command pic1
#define pic1Data pic1+1
#define pic2Command pic2
#define pic2Data pic2+1

#define picEOI 0x20

#define picICW1_ICW4 0x01
#define picICW1Init 0x10
#define picICW4_8086 0x01

void picSendEOI(uint8_t port){
  if(port>=8){
    outb(pic2Command, picEOI);
  }
  outb(pic1Command, picEOI);
}

void picRemap(uint32_t offset1, uint32_t offset2){
  uint8_t mask1 = inb(pic1Data);
  uint8_t mask2 = inb(pic2Data);

  outb(pic1Command, picICW1Init|picICW1_ICW4);
  outb(pic2Command, picICW1Init|picICW1_ICW4);
  outb(pic1Data, offset1);
  outb(pic2Data, offset2);
  outb(pic1Data, 4);
  outb(pic2Data, 2);

  outb(pic1Data, picICW4_8086);
  outb(pic2Data, picICW4_8086);

  outb(pic1Data, mask1);
  outb(pic2Data, mask2);
}

