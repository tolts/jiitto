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

#include "algo/char.h"
#include "algo/conversion.h"

#include "drivers/drivers.h"
#include "drivers/vga.h"

#include "int/int.h"
#include "int/idt.h"
#include "int/pic.h"
#include "int/isr.h"

#include "drivers/keyboard.h"

void init(void){

	isrInstall();
	setIdtr();
	setIdt((unsigned int) &IDTR);

    cursorEnable(0, 15);

    keyboardInit();

    kprint("brrrrrrrrrtr. Here is the cursor ->\0", 190, 0x0E);

	while(1){}
}
