
# Path to OVMF_CODE.fd and OVMF_VARS.fd
OVMF_CODE=/usr/share/edk2/ovmf/OVMF_CODE.fd
OVMF_VARS=/usr/share/edk2/ovmf/OVMF_VARS.fd


emul: build
	qemu-system-x86_64 -enable-kvm \
		-drive if=pflash,format=raw,readonly=on,file=$(OVMF_CODE) \
		-drive if=pflash,format=raw,readonly=on,file=$(OVMF_VARS) \
		-drive format=raw,file=fat:rw:esp

build:
	cargo build --target x86_64-unknown-uefi
	mkdir -p esp/efi/boot
	cp target/x86_64-unknown-uefi/debug/jiitto.efi esp/efi/boot/BOOTX64.efi

default all: emul
