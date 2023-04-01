#include "drivers/drivers.c"
#include "drivers/vga.c"

#include "algo/char.c"
#include "algo/conversion.c"

#include "int/int.c"
#include "int/idt.c"
#include "int/pic.c"
#include "int/isr.c"

#include "drivers/keyboard.c"

void init(void){

	isrInstall();
	setIdtr();
	setIdt((unsigned int) &IDTR);

	//initKeyboard();

	kprint(convertIntAscii(125), 0, 0x0F);

	while(0){}
}
