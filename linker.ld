
ENTRY(START)
OUTPUT_FORMAT("binary")
INPUT(boot.o)

SECTIONS{
  . = 0x7C00;
  .boot : AT(0x7C00) SUBALIGN(4){
    __boot_start = .;
    *(.boot)
  }
  . = 0x100000;
  .text BLOCK(4K) : AT(0x100000) ALIGN(4K){
    *(.text)
  }
  .rodata BLOCK(4K) : ALIGN(4K){
    *(.rodata)
  }
  .data BLOCK(4K) : ALIGN(4K){
    *(.data)
  }
  .bss BLOCK(4K) : ALIGN(4K){
    *(.bss)
  }
}

OUTPUT(boot.img)
