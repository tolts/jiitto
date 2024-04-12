
#include "lib/def.c"
#include "kernel/graphics.c"
#include "kernel/interrupts.c"
#include "kernel/cursor.c"
#include "kernel/keyboard.c"

void kernel_main(void){

// initialisation
  idtInit("idt set\0");
  keyboardInit("keyboard init\0");
  cursorEnable(0x0, 0xF);

  cursorPosition=cursorGetPosition();


// core terminal support
  uint16_t* buffer;
  kprint(">\0", cursorPosition++, __WHITE__, __BLACK__);
}

