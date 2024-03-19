
#include "lib/def.c"
#include "kernel/graphics.c"
#include "kernel/interrupts.c"

void kernel_main(void){
  //kprintc('X', vgaCoordinatePlanToLine(2, 2), __BLACK__, __WHITE__);
  kprint("Hello\0", 0, __BLUE__, __RED__);


  idtInit();
  //kprint("idt set\0", 17, __BLUE__, __RED__);

  //__asm__ __volatile__ ("int $0");
  __HALT__;
//  kprint("int\0", 30, __BLUE__, __RED__);

  return;
}

