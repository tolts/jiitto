
/*
* Only for kernel/core. Contains useful definitions, constants, etc.
*/



#pragma once

// type def

typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
//typedef void* uintptr_t;

static inline uint32_t core_str_len(uint8_t* string){
  uint32_t len;
  while(string[++len]);
  return len;
}

// toolings

#define HALT __asm__ __volatile__ ("hlt")

// constants

#define TRUE 1
#define FALSE 0

#define GDT_CODE_SEGMENT 0x08
#define GDT_DATA_SEGMENT 0x10

// io

static inline void core_outb(uint16_t port, uint8_t data){
  __asm__ __volatile__ ("outb %b0, %w1" : : "a"(data), "Nd"(port) : "memory");
  return;
}

static inline uint8_t core_inb(uint16_t port){
  uint8_t data;
  __asm__ __volatile__ ( "inb %w1, %b0":"=a"(data):"Nd"(port):"memory");
  return data;
}

static inline void core_io_wait(void){
  core_outb(0x80, 0);
  return;
}

