
RUNPATH=$(shell cat path.txt)# add path.txt next to makefile with path to compiler
NAME=shisaku
IMGTYPE=iso
OS=$(NAME).$(IMGTYPE)

clean:
	rm -r isodir
	rm boot.o kernel.o $(NAME).bin

run:
	make build
	$(RUNPATH)

build:
	if [ -e $(OS) ]; then rm $(OS); fi
	i686-elf-as boot.s -o boot.o
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -T linker.ld -o $(NAME).bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
	if ! grub-file --is-x86-multiboot $(NAME).bin; then exit; fi
	mkdir -p isodir/boot/grub
	cp $(NAME).bin isodir/boot/$(NAME).bin
	touch isodir/boot/grub/grub.cfg
	echo '' >> isodir/boot/grub/grub.cfg
	echo 'menuentry "$(NAME)" {' >> isodir/boot/grub/grub.cfg
	echo '	multiboot /boot/$(NAME).bin' >> isodir/boot/grub/grub.cfg
	echo '	boot' >> isodir/boot/grub/grub.cfg
	echo '}' >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(OS) isodir
	make clean