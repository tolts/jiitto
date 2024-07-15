
__RUN__=$(shell cat run.txt)

run:
	nasm -f elf32 kernel/boot.s -o boot.o
	i686-elf-gcc -c kernel/kernel.c -o kernel.o -ffreestanding
	i686-elf-ld -T linker.ld
	$(__RUN__)

