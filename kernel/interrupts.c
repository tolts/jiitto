
#pragma once

#include "../lib/def.c"
#include "graphics.c"

// everything interrupts

// PIC

#define __PIC1__ 0x20
#define __PIC2__ 0xA0
#define __PIC1_COMMAND__ __PIC1__
#define __PIC2_COMMAND__ __PIC2__
#define __PIC1_DATA__ (__PIC1__+1)
#define __PIC2_DATA__ (__PIC2__+1)

#define __PIC_EOI__ 0x20

#define __PIC1_OFFSET__ 0x20
#define __PIC2_OFFSET__ 0x28

void picEOI(uint8_t irq){
  if(irq>=8){
    outb(__PIC2_COMMAND__, __PIC_EOI__);
  }
  outb(__PIC1_COMMAND__, __PIC_EOI__);
  return;
}

#define __ICW1_ICW4__ 0x01
#define __ICW1_SINGLE__ 0x02
#define __ICW1_INTERVAL4__ 0x04
#define __ICW1_LEVEL__ 0x08
#define __ICW1_INIT__ 0x10

#define __ICW4_8086MODE__ 0x01
#define __ICW4_AUTO__ 0x02
#define __ICW4_BUFFER_SLAVE_MODE__ 0x08
#define __ICW4_BUFFER_MASTER_MODE__ 0x0C
#define __ICW4_SPECIAL_FULLY_NESTED__ 0x10

void picRemap(uint32_t offset1, uint32_t offset2){
  uint8_t mask1=inb(__PIC1_DATA__);
  uint8_t mask2=inb(__PIC2_DATA__);
  outb(__PIC1_COMMAND__, __ICW1_INIT__|__ICW1_ICW4__);
  ioWait();
  outb(__PIC2_COMMAND__, __ICW1_INIT__|__ICW1_ICW4__);
  ioWait();
  outb(__PIC1_DATA__, offset1);
  ioWait();
  outb(__PIC2_DATA__, offset2);
  ioWait();
  outb(__PIC1_DATA__, 4);
  ioWait();
  outb(__PIC2_DATA__, 2);
  ioWait();
  outb(__PIC1_DATA__, __ICW4_8086MODE__);
  ioWait();
  outb(__PIC2_DATA__, __ICW4_8086MODE__);
  ioWait();
  outb(__PIC1_DATA__, mask1);
  ioWait();
  outb(__PIC2_DATA__, mask2);
  ioWait();
  return;
}

// APIC

// NMI

// IDT

typedef struct{
  uint16_t offset0_15;
  uint16_t segmentSelector16_31;
  uint8_t reserved32_39;
  uint8_t typeAttributes40_47;
  uint16_t offset48_63;
}__attribute__((packed)) idtSegmentDescriptor_t;

typedef struct{
  uint16_t size;
  uint32_t offset;
}__attribute__((packed)) idtDescriptor_t;

__attribute__((aligned(0x10))) static idtSegmentDescriptor_t idt[256];

static idtDescriptor_t idtr;

extern void* ISR_STUB_TABLE[48];

void idtSetSegmentDescriptor(uint8_t vector, void* isr, uint8_t flags){
  idtSegmentDescriptor_t* segmentDescriptor=&idt[vector];
  segmentDescriptor->offset0_15=(uint32_t)isr&0xFFFF;
  segmentDescriptor->segmentSelector16_31=__gdtCodeSegment__;//GDT_CODE_SEGMENT;
  segmentDescriptor->reserved32_39=0;
  segmentDescriptor->typeAttributes40_47=flags;
  segmentDescriptor->offset48_63=(uint32_t)isr>>16;
}

void idtInit(){
  idtr.offset=(uint32_t)&idt[0];
  idtr.size=(uint16_t)sizeof(idtSegmentDescriptor_t)*256-1;
  for(uint8_t vector=0; vector<32; vector++){
    idtSetSegmentDescriptor(vector, ISR_STUB_TABLE[vector], 0x8E);
  }
  picRemap(__PIC1_OFFSET__, __PIC2_OFFSET__);
  for(uint8_t vector=0; vector<16; vector++){
    idtSetSegmentDescriptor(vector+32, ISR_STUB_TABLE[vector+32], 0x8E);
  }
  __asm__ __volatile__ ("cli; lidt (%0); sti"::"r"(&idtr));
  return;
}

// exceptions

typedef struct{
  uint32_t ds, edi, esi, ebp, esp, ebx, edx, ecx, eax;
}__attribute__((packed)) isrCpuState_t;

typedef struct{
  uint32_t number, errorcode, eip, cs, eflags, useresp, ss;
}__attribute__((packed)) isrStackState_t;

void exceptionHandler(isrCpuState_t cpu, isrStackState_t stack){
  kprint((uint8_t*)"interrupt detected!\0", 100, __WHITE__, __BLACK__);
  kprintc(((uint16_t)(stack.number))+'0', 0, __WHITE__, __BLACK__);
  kprintc(((uint16_t)(stack.errorcode))+'0', 120, __WHITE__, __BLACK__);
  return;
}

void irqHandler(isrCpuState_t cpu, isrStackState_t stack){
  kprint((uint8_t*)"IRQ detected!\0", 180, __WHITE__, __BLACK__);
  kprintc(((uint16_t)(stack.number)), 80, __WHITE__, __BLACK__);
  kprintc(((uint16_t)(stack.errorcode)), 200, __WHITE__, __BLACK__);
  return;
}

