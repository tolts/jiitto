
#include "misc/types.c"
#include "misc/io.c"

#include "cpu/gdt.c"
#include "cpu/pic.c"
#include "cpu/isr.c"
#include "cpu/isrDef.h"//ignore error
#include "cpu/idt.c"

#define VGA_ADDRESS 0xB8000

#define VGA_COLOUR_BLACK 0
#define VGA_COLOUR_BLUE 1
#define VGA_COLOUR_GREEN 2
#define VGA_COLOUR_CYAN 3
#define VGA_COLOUR_RED 4
#define VGA_COLOUR_MAGENTA 5
#define VGA_COLOUR_BROWN 6
#define VGA_COLOUR_GREY_LIGHT 7
#define VGA_COLOUR_GREY_DARK 8
#define VGA_COLOUR_BLUE_LIGHT 9
#define VGA_COLOUR_GREEN_LIGHT 10
#define VGA_COLOUR_CYAN_LIGHT 11
#define VGA_COLOUR_RED_LIGHT 12
#define VGA_COLOUR_MAGENTA_LIGHT 13
#define VGA_COLOUR_BROWN_LIGHT 14
#define VGA_COLOUR_WHITE 15

uint8_t VGA_COLOUR_NOW = (VGA_COLOUR_BLACK<<4)|(VGA_COLOUR_WHITE);

void changeTextColourt(uint8_t bg, uint8_t fg){
  VGA_COLOUR_NOW = (bg<<4)|(fg);
}

uint8_t getTextColourSelectiont(void){
  return VGA_COLOUR_NOW;
}

void cleart(void){
  uint8_t* VGA = (uint8_t*)VGA_ADDRESS;
  for(int16_t i = 1; i<=2000; i++){
    *VGA++ = VGA_COLOUR_BLACK;
    *VGA++ = VGA_COLOUR_WHITE;
  }
}

void printt(int8_t* t, int16_t x){
  int8_t* VGA = (int8_t*)VGA_ADDRESS + (x<<2);
  while(*t != '\0'){
    *VGA++ = *t++;
    *VGA++ = VGA_COLOUR_NOW;
  }
}

void kernelInit(void){
  gdtEncode();
  printt("gdt encoded\0", 0);

  idtEncode();
  printt("idt encoded\0", 40);

  while(1);
  //__asm__ __volatile__ ("int $0");
  __asm__ __volatile__ ("hlt");


}

