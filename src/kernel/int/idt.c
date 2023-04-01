#pragma once

extern void setIdt(unsigned int idtr);

typedef struct{
	unsigned short offset1;
	unsigned short segmentSelector;
	unsigned char reserved;
	unsigned char flag;
	unsigned short offset2;
}__attribute__((packed)) idtg;

typedef struct{
	unsigned short size;
	unsigned int offset;
}__attribute__((packed)) idtr;

idtg IDT[256];
idtr IDTR;

void setIdtGate(unsigned char n, unsigned int isr){
	IDT[n].offset1 = (unsigned short)(isr & 0xFFFF);
	IDT[n].segmentSelector = 0x08;
	IDT[n].reserved = 0;
	IDT[n].flag = 0b10001110;
	IDT[n].offset2 = (unsigned short)((isr >> 16));
}

void setIdtr(void){
	IDTR.size = (sizeof(idtg) * 256) - 1;
	IDTR.offset = (unsigned int)&IDT;
}
