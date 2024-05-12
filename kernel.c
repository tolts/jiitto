
//#define __LOG__ // define to have all logs

#include "lib/def.c"
#include "lib/printf.c"

// general definitions

uint16_t terminalBufferStartPoint;
uint16_t terminalBufferEndPoint;

uint8_t* terminalHeader="@\0";

#include "kernel/core/cursor.c"
#include "kernel/core/graphics.c"
#include "kernel/core/interrupts.c"
#include "kernel/core/keyboard.c"
#include "kernel/core/terminal.c"

// kernel initialisation (main loop is in boot.s)

void kernel_main(void){

  idtInit(
#ifdef __LOG__
    "interrupts enabled\0"
#endif
);
  keyboardInit(
#ifdef __LOG__
    "keyboard enabled\0"
#endif
);

  cursorEnable(0x0, 0xF);
  cursorPosition=cursorGetPosition();
  terminalInit(terminalHeader);
}

