
#include "lib/def.c"
#include "kernel/graphics.c"
#include "kernel/interrupts.c"
#include "kernel/keyboard.c"

void kernel_main(void){
  kprintc('X', vgaCoordinatePlanToLine(2, 2), __BLACK__, __WHITE__);
  kprint("Hello \0", 0, __BLUE__, __RED__);


  idtInit("idt set");
  keyboardInit("keyboard init");


}

