
ENTRY(START)
OUTPUT_FORMAT("binary")
INPUT(boot.o kernel.o sys.o)

SECTIONS{
  . = 0x7C00;
  .boot : AT(0x7C00) SUBALIGN(4){
    *(.boot)
  }
  .text BLOCK(4K):ALIGN(4K) {
    *(.text)
  }
  .rodata BLOCK(4K):ALIGN(4K) {
    *(.rodata)
  }
  .data BLOCK(4K):ALIGN(4K) {
    *(.data)
  }
  .bss BLOCK(4K):ALIGN(4K) {
    *(.bss)
  }
}

OUTPUT(boot.img)

