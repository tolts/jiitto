/*
    Copyright (C) 2023  schochtlts

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "drivers/drivers.c"
#include "drivers/vga.c"

#include "algo/char.c"
#include "algo/conversion.c"

#include "int/int.c"
#include "int/idt.c"
#include "int/pic.c"
#include "int/isr.c"

//#include "drivers/keyboard.c"

void init(void){

/*	isrInstall();
	setIdtr();
	setIdt((unsigned int) &IDTR);*/

	//initKeyboard();

	//kprint(convertIntAscii(125), 0, 0x0F);

	while(0){}
}
