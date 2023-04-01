#pragma once

void keyboardHandler(registers regs){
    kprint((const char*)portByteIn(ps2KeyboardDataPort), 0, 0x0F);
    picAcknowledge(irqKeyboard);
}

void initKeyboard(void){
//    picDisable();
//    picClearMask(irqKeyboard);
//    picRemap(picMasterOffset, picSlaveOffset);
    setIdtGate(irqKeyboard + 32, (unsigned int)&keyboardHandler);
}
