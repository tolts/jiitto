
build_dir=build
run_vm=cmd.exe /C qemu-system-x86_64 -cpu qemu64 -net none -pflash OVMF-pure-efi.fd -cdrom jiitto.iso

TARGET=$(build_dir)/BOOTX64.efi
SRCS=src/main.c
include uefi/Makefile
