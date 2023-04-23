#pragma once

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

isr intHandlers[256];

void isrHandler(registers regs){
	char* intNumber = intToAscii((int)regs.intNumber, 10);
	kprint("interrupt \0", 0, 0x0A);
	kprint(charAddNull(intNumber), 10, 0x0E);
	kprint(" received\0", 10 + charLen(intNumber), 0x0A);
}

void irqHandler(registers regs){
	picAcknowledge(regs.intNumber - 32);
	if(intHandlers[regs.intNumber] != 0){
		intHandlers[regs.intNumber](regs);
	}
}

void setIntHandler(unsigned char n, isr handler){
	intHandlers[n] = handler;
}

void isrInstall(void){
	setIdtGate(0, (unsigned int)&isr0);
	setIdtGate(1, (unsigned int)&isr1);
	setIdtGate(2, (unsigned int)&isr2);
	setIdtGate(3, (unsigned int)&isr3);
	setIdtGate(4, (unsigned int)&isr4);
	setIdtGate(5, (unsigned int)&isr5);
	setIdtGate(6, (unsigned int)&isr6);
	setIdtGate(7, (unsigned int)&isr7);
	setIdtGate(8, (unsigned int)&isr8);
	setIdtGate(9, (unsigned int)&isr9);
	setIdtGate(10, (unsigned int)&isr10);
	setIdtGate(11, (unsigned int)&isr11);
	setIdtGate(12, (unsigned int)&isr12);
	setIdtGate(13, (unsigned int)&isr13);
	setIdtGate(14, (unsigned int)&isr14);
	setIdtGate(15, (unsigned int)&isr15);
	setIdtGate(16, (unsigned int)&isr16);
	setIdtGate(17, (unsigned int)&isr17);
	setIdtGate(18, (unsigned int)&isr18);
	setIdtGate(19, (unsigned int)&isr19);
	setIdtGate(20, (unsigned int)&isr20);
	setIdtGate(21, (unsigned int)&isr21);
	setIdtGate(22, (unsigned int)&isr22);
	setIdtGate(23, (unsigned int)&isr23);
	setIdtGate(24, (unsigned int)&isr24);
	setIdtGate(25, (unsigned int)&isr25);
	setIdtGate(26, (unsigned int)&isr26);
	setIdtGate(27, (unsigned int)&isr27);
	setIdtGate(28, (unsigned int)&isr28);
	setIdtGate(29, (unsigned int)&isr29);
	setIdtGate(30, (unsigned int)&isr30);
	setIdtGate(31, (unsigned int)&isr31);

	picRemap(picMasterOffset, picSlaveOffset);

	setIdtGate(32, (unsigned int)&irq0);
	setIdtGate(33, (unsigned int)&irq1);
	setIdtGate(34, (unsigned int)&irq2);
	setIdtGate(35, (unsigned int)&irq3);
	setIdtGate(36, (unsigned int)&irq4);
	setIdtGate(37, (unsigned int)&irq5);
	setIdtGate(38, (unsigned int)&irq6);
	setIdtGate(39, (unsigned int)&irq7);
	setIdtGate(40, (unsigned int)&irq8);
	setIdtGate(41, (unsigned int)&irq9);
	setIdtGate(42, (unsigned int)&irq10);
	setIdtGate(43, (unsigned int)&irq11);
	setIdtGate(44, (unsigned int)&irq12);
	setIdtGate(45, (unsigned int)&irq13);
	setIdtGate(46, (unsigned int)&irq14);
	setIdtGate(47, (unsigned int)&irq15);
}
