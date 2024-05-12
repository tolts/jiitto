
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

uint32_t strLen(uint8_t* string){
  uint32_t len;
  while(string[++len]);
  return len;
}

uint8_t strEq(uint8_t* A, uint8_t* B){
  if(strLen(A)==strLen(B)){
    return 0;
  }else{/*
    while(*A!='\0'){
      if(*A!=*B){
        return 0;
      }
      A++;
      B++;
    }*/
  }
  return 1;
}

// toolings

#define __HALT__ __asm__ __volatile__ ("hlt")

// constants

#define __TRUE__ 1
#define __FALSE__ 0

#define __GDT_CODE_SEGMENT__ 0x08
#define __GDT_DATA_SEGMENT__ 0x10

// io

static inline void outb(uint16_t port, uint8_t data){
  __asm__ __volatile__ ("outb %b0, %w1" : : "a"(data), "Nd"(port) : "memory");
  return;
}

static inline uint8_t inb(uint16_t port){
  uint8_t data;
  __asm__ __volatile__ ( "inb %w1, %b0":"=a"(data):"Nd"(port):"memory");
  return data;
}

static inline void ioWait(void){
  outb(0x80, 0);
  return;
}

