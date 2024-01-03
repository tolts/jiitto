
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
	if [ -e cpu/isrDef.h]; then rm cpu/isrDef.h; fi
	touch cpu/isrDef.h
	echo '#define isrGen isrHandler[0] = isr0;\' >> cpu/isrDef.h
	for i in $(shell seq 1 30); do echo 'isrHandler['$$i'] = isr'$$i';\' >> cpu/isrDef.h; done
	echo 'isrHandler[31] = isr31;' >> cpu/isrDef.h
	echo '#define irqGen isrHandler[32] = isr32;\' >> cpu/isrDef.h
	for i in $(shell seq 33 46); do echo 'isrHandler['$$i']=isr'$$i';\' >> cpu/isrDef.h; done
	echo 'isrHandler[47]=isr47;' >> cpu/isrDef.h
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

