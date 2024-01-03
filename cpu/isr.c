
#pragma once

#include "../misc/types.c"

void isrExceptionHandler(uint32_t intExceptionNumber, uint32_t intExceptionErrorCode);

void printt(int8_t* t, int16_t x);

void* isrHandler[48];

#define isrErrorStub(n) __attribute__((noreturn)) void isr##n (void){\
  uint32_t exceptionErrorCode;\
  __asm__ __volatile__ ("pop %0":"=r"(exceptionErrorCode):);\
  isrExceptionHandler(n, exceptionErrorCode);\
  __asm__ __volatile__ ("iret");\
  while(1);\
}

#define isrNoErrorStub(n) __attribute__((noreturn)) void isr##n (void){\
  isrExceptionHandler(n, 0);\
  __asm__ __volatile__ ("iret");\
  while(1);\
}

void isrExceptionHandler(uint32_t intExceptionNumber, uint32_t intExceptionErrorCode){
  int8_t* exceptionNumber = typesInt2CharReversed(intExceptionNumber, 10);
  int8_t* errorCode = typesInt2CharReversed(intExceptionErrorCode, 10);
  int8_t msg[] = "Exception (interrupt) '__' errorcode='__' \0";
  msg[23] = exceptionNumber[0];
  msg[24] = exceptionNumber[1];
  msg[38] = errorCode[0];
  msg[39] = errorCode[1];
  printt(msg, 0);
}

//isr
isrNoErrorStub(0);
isrNoErrorStub(1);
isrNoErrorStub(2);
isrNoErrorStub(3);
isrNoErrorStub(4);
isrNoErrorStub(5);
isrNoErrorStub(6);
isrNoErrorStub(7);
isrErrorStub(8);
isrNoErrorStub(9);
isrErrorStub(10);
isrErrorStub(11);
isrErrorStub(12);
isrErrorStub(13);
isrErrorStub(14);
isrNoErrorStub(15);
isrNoErrorStub(16);
isrErrorStub(17);
isrNoErrorStub(18);
isrNoErrorStub(19);
isrNoErrorStub(20);
isrNoErrorStub(21);
isrNoErrorStub(22);
isrNoErrorStub(23);
isrNoErrorStub(24);
isrNoErrorStub(25);
isrNoErrorStub(26);
isrNoErrorStub(27);
isrNoErrorStub(28);
isrNoErrorStub(29);
isrNoErrorStub(30);
isrNoErrorStub(31);

//irq
isrErrorStub(32);
isrErrorStub(33);
isrErrorStub(34);
isrErrorStub(35);
isrErrorStub(36);
isrErrorStub(37);
isrErrorStub(38);
isrErrorStub(39);
isrErrorStub(40);
isrErrorStub(41);
isrErrorStub(42);
isrErrorStub(43);
isrErrorStub(44);
isrErrorStub(45);
isrErrorStub(46);
isrErrorStub(47);

