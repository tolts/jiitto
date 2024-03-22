
__RUN__=$(shell cat run.txt)

run:
	nasm -f elf32 boot.s -o boot.o
	i686-elf-gcc -c kernel.c -o kernel.o -ffreestanding
	i686-elf-ld -T linker.ld
	$(__RUN__)

