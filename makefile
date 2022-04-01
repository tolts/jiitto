helios: 
	nasm -f bin bootsector.asm -o bootsector.bin
	nasm -f bin bootloader.asm -o bootloader.bin

	nasm -f elf64 extern.asm -o extern.o
	gcc -ffreestanding -c kernel.c -o kernel.o
	ld -o firmware.bin -Ttext 0xE000 extern.o kernel.o --oformat binary

	cat bootsector.bin bootloader.bin firmware.bin > helios.img

mrpropre: 
	rm *.bin *.o *.img