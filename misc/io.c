
#pragma once

#include "types.c"

static inline void outb(uint16_t port, uint8_t data){
  __asm__ __volatile__ ("outb %0, %1"::"a"(data), "Nd"(port):"memory");
}

static inline uint8_t inb(uint16_t port){
  uint8_t data;
  __asm__ __volatile__ ("inb %1, %0":"=a"(data):"Nd"(port):"memory");
  return data;
}

static inline void ioWait(void){
  outb(0x80, 0);
}
