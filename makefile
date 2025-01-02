
build_dir=build
run_vm=cmd.exe /C qemu-system-x86_64 -cpu qemu64 -net none -bios OVMF-pure-efi.fd -drive format=raw,index=0,media=cdrom,file=jiitto.iso

TARGET=$(build_dir)/BOOTX64.efi
SRCS=src/main.c
TO_RM= src/main.o
include uefi/Makefile
