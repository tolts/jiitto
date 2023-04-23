#pragma once

#define ps2KeyboardEnablePort1 0xAE
#define ps2KeyboardReadControllerConfigurationByte 0x20
#define ps2KeyboardWriteControllerConfigurationByte 0x60

#define ps2KeyboardDataPort 0x60
#define ps2KeyboardStatusAndCommandPort 0x64

#define irqKeyboard 33

void portByteOut(unsigned short port, unsigned char data){
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

char portByteIn(unsigned short port){
    unsigned char data;
    __asm__("in %%dx, %%al" : "=a" (data) : "d" (port));
    return data;
}

void portWait(void){
    portByteOut(0x80, 0);
}