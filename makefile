DIR=$(shell cat dir.txt)
DEBUG=$(shell cat debug.txt)

run:
	nasm src/kernel/boot/bootloader.asm -f bin -o bootloader.bin
	nasm src/kernel/entry.asm -f elf32 -o entry.o -i $(DIR)/src/kernel/
	i686-elf-gcc -c src/kernel/init.c -o main.o -ffreestanding
	i686-elf-ld -o kernel.bin -Ttext 0x8200 entry.o main.o --oformat binary
	cat bootloader.bin kernel.bin > os.img
	rm *.bin *.o
	$(DEBUG)
