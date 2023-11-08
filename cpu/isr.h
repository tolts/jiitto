
#pragma once

__attribute__((noreturn)) void isrExceptionHandler(void){
  __asm__ __volatile__ ("cli; hlt");
}
