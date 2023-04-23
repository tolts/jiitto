#pragma once

//keyboardScancodeSet1

void keyboardHandler(registers regs){
    kprint(charAddNull(intToAscii(portByteIn(0x60), 16)), 80, 0x0F);
}

void keyboardInit(void){
    setIntHandler(irqKeyboard, keyboardHandler);
}
