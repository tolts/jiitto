#	makefile
#   Copyright (C) 2022 schochtlts
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <https://www.gnu.org/licenses/>.

#	Working directory ERASE THE VALUES GIVEN TO DIR BEFORE COMMITING ANY CHANGES.
DIR=

os: 
#	bootloader
	nasm $(DIR)/bootloader/bootsector.asm -f bin -o $(DIR)/.build/bootsector.bin
	nasm $(DIR)/bootloader/bootloader.asm -f bin -o $(DIR)/.build/bootloader_.bin
	cat $(DIR)/.build/bootsector.bin $(DIR)/.build/bootloader_.bin > $(DIR)/.build/bootloader.bin

#	kernel
	nasm $(DIR)/kernel/entry.asm -f elf64 -o $(DIR)/.build/entry.o
	x86_64-elf-gcc -ffreestanding -mno-red-zone -m64 -c $(DIR)/kernel/kernel.c -o $(DIR)/.build/kernel.o
	ld -o $(DIR)/.build/kernel.bin -Ttext 0x8200 $(DIR)/.build/entry.o $(DIR)/.build/kernel.o --oformat binary

#	kernel + bootloader
	cat $(DIR)/.build/bootloader.bin $(DIR)/.build/kernel.bin > $(DIR)/.build/os.img

clean: 
	rm $(DIR)/.build/*.o $(DIR)/.build/*.img $(DIR)/.build/*.bin

mrpropre: 
	make clean
