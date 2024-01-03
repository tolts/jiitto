
#pragma once

#include "../misc/types.c"

typedef struct{
  uint16_t offset0_15;
  uint16_t segmentSelector16_31;
  uint8_t reserved32_39;
  uint8_t typeAttributes40_47;
  uint16_t offset48_63;
}__attribute__((packed)) idtSegmentDescriptor_t;

typedef struct{
  uint16_t size;
  idtSegmentDescriptor_t* offset;
}__attribute__((packed)) idtDescriptor_t;

__attribute__((aligned(0x10))) static idtSegmentDescriptor_t idt[256];

idtDescriptor_t idtr = {(uint16_t)sizeof(idt)*256 - 1, idt};

void idtEncodeSegmentDescriptor(uint8_t vector, void* isr, uint8_t flags){
  idtSegmentDescriptor_t* segmentDescriptor = &idt[vector];
  segmentDescriptor->offset0_15 = (uint32_t)isr & 0xFFFF;
  segmentDescriptor->segmentSelector16_31 = gdtCodeSelector;
  segmentDescriptor->reserved32_39 = 0;
  segmentDescriptor->typeAttributes40_47 = flags;
  segmentDescriptor->offset48_63 = (uint32_t)isr >> 16;
}


void idtEncode(void){
  isrGen //ignore error
  for(uint8_t vector = 0; vector < 32; vector++){
    idtEncodeSegmentDescriptor(vector, isrHandler[vector], 0x8E);
  }
  picRemap(0x20, 0x28);
  irqGen //ignore error
  for(uint8_t vector = 47; vector >= 32; vector--){
    idtEncodeSegmentDescriptor(vector, isrHandler[vector], 0x8E);
  }

  __asm__ __volatile__ ("cli; lidt (%0); sti"::"r"(&idtr));
  return;
}

