
#pragma once

#include "../../include/core.h"
#include "graphics.c"

// everything interrupts

// PIC

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC2_COMMAND PIC2
#define PIC1_DATA (PIC1+1)
#define PIC2_DATA (PIC2+1)

#define PIC_EOI 0x20

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

void core_pic_eoi(uint8_t irq){
  if(irq>=8){
    core_outb(PIC2_COMMAND, PIC_EOI);
  }
  core_outb(PIC1_COMMAND, PIC_EOI);
  return;
}

#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT 0x10

#define ICW4_8086MODE 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUFFER_SLAVE_MODE 0x08
#define ICW4_BUFFER_MASTER_MODE 0x0C
#define ICW4_SPECIAL_FULLY_NESTED 0x10

void core_pic_remap(uint32_t offset1, uint32_t offset2){
  uint8_t mask1=core_inb(PIC1_DATA);
  uint8_t mask2=core_inb(PIC2_DATA);
  core_outb(PIC1_COMMAND, ICW1_INIT|ICW1_ICW4);
  core_io_wait();
  core_outb(PIC2_COMMAND, ICW1_INIT|ICW1_ICW4);
  core_io_wait();
  core_outb(PIC1_DATA, offset1);
  core_io_wait();
  core_outb(PIC2_DATA, offset2);
  core_io_wait();
  core_outb(PIC1_DATA, 4);
  core_io_wait();
  core_outb(PIC2_DATA, 2);
  core_io_wait();
  core_outb(PIC1_DATA, ICW4_8086MODE);
  core_io_wait();
  core_outb(PIC2_DATA, ICW4_8086MODE);
  core_io_wait();
  core_outb(PIC1_DATA, mask1);
  core_io_wait();
  core_outb(PIC2_DATA, mask2);
  core_io_wait();
  return;
}

// APIC

// NMI

// IDT

typedef struct{
  uint16_t offset_0_15;
  uint16_t segment_selector_16_31;
  uint8_t reserved_32_39;
  uint8_t type_attributes_40_47;
  uint16_t offset_48_63;
}__attribute__((packed)) core_idt_segment_descriptor_t;

typedef struct{
  uint16_t size;
  uint32_t offset;
}__attribute__((packed)) core_idt_descriptor_t;

//__attribute__((aligned(0x10))) 
static core_idt_segment_descriptor_t idt[256];

static core_idt_descriptor_t idtr;

extern void* core_isr_stub_table[48];

void core_idt_set_segment_descriptor(uint8_t vector, void* isr, uint8_t flags){
  core_idt_segment_descriptor_t* segment_descriptor=&idt[vector];
  segment_descriptor->offset_0_15=(uint32_t)isr&0xFFFF;
  segment_descriptor->segment_selector_16_31=GDT_CODE_SEGMENT;
  segment_descriptor->reserved_32_39=0;
  segment_descriptor->type_attributes_40_47=flags;
  segment_descriptor->offset_48_63=(uint32_t)isr>>16;
  return;
}

void core_idt_init(
#ifdef LOG
  uint8_t* message
#endif
){
  idtr.offset=(uint32_t)&idt[0];
  idtr.size=(uint16_t)sizeof(core_idt_segment_descriptor_t)*256-1;
  for(uint8_t vector=0; vector<32; vector++){
    core_idt_set_segment_descriptor(vector, core_isr_stub_table[vector], 0x8E);
  }
  core_pic_remap(PIC1_OFFSET, PIC2_OFFSET);
  for(uint8_t vector=0; vector<16; vector++){
    core_idt_set_segment_descriptor(vector+32, core_isr_stub_table[vector+32], 0x8E);
  }
  core_outb(0x21,0xFD);core_outb(0xA1,0xFF);
  __asm__ __volatile__ ("cli; lidt (%0); sti"::"r"(&idtr));
#ifdef LOG
  core_log_str(message, core_cursor_position, WHITE, BLACK);
#endif
  return;
}

// exceptions

typedef struct{
  uint32_t ds, edi, esi, ebp, esp, ebx, edx, ecx, eax;
}__attribute__((packed)) core_isr_cpu_state_t;

typedef struct{
  uint32_t number, errorcode, eip, cs, eflags, useresp, ss;
}__attribute__((packed)) core_isr_stack_state_t;

void core_exception_handler(core_isr_cpu_state_t cpu, core_isr_stack_state_t stack){
  core_log_str((uint8_t*)"interrupt detected!\0", core_cursor_position, RED, WHITE);
  core_log(((uint16_t)(stack.number))+'0', *core_cursor_position, RED, WHITE);
  core_log(((uint16_t)(stack.errorcode))+'0', *core_cursor_position, RED, WHITE);
  HALT;
  return;
}

void core_irq_handler(core_isr_cpu_state_t cpu, core_isr_stack_state_t stack){
  core_log_str((uint8_t*)"IRQ detected!\0", core_cursor_position, WHITE, BLACK);
  core_log(((uint16_t)(stack.number)), *core_cursor_position, WHITE, BLACK);
  core_log(((uint16_t)(stack.errorcode)), *core_cursor_position, WHITE, BLACK);
  return;
}

