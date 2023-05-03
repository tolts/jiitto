#pragma once

void keyboardHandler(registers regs){
    unsigned char scancode = portByteIn(0x60);
    switch(scancode){
        case 0x2:
            kprint("&\0", cursorPos(), 0x0A);
            return;
        default: 
            return;
    }
}

void keyboardInit(void){
    setIntHandler(irqKeyboard, keyboardHandler);
}
