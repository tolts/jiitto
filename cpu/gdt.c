
#pragma once

#define gdtCodeSelector 0x8
#define gdtDataSelector 0x10

#include "../misc/types.c"

typedef struct{
    uint16_t limit0_15;
    uint16_t base16_31;
    uint8_t base32_39;
    uint8_t accessByte40_47;
    uint8_t limit48_51Flags52_55;
    uint8_t base56_63;
}__attribute__((__packed__)) gdtSegmentDescriptor_t;

typedef struct{
    uint16_t size;
    gdtSegmentDescriptor_t* offset;
}__attribute__((__packed__)) gdtDescriptor_t;

gdtSegmentDescriptor_t gdt[] = {
  {0, 0, 0, 0, 0, 0},
  {0xFFFF, 0, 0, 0x9A, 0xCF, 0},
  {0xFFFF, 0, 0, 0x92, 0xCF, 0}
};

gdtDescriptor_t gdtr = {sizeof(gdt) - 1, gdt};

void gdtEncode(void){
  __asm__ __volatile__ ("lgdt (%0)"::"r"(&gdtr));
}

