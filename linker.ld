
ENTRY(START)
OUTPUT_FORMAT("binary")
INPUT(boot.o kernel.o)

SECTIONS{
  . = 0x7C00;
  .boot : AT(0x7C00) SUBALIGN(4){
    *(.boot)
  }
  .text : {
    *(.text)
  }
  .rodata : {
    *(.rodata)
  }
  .data : {
    *(.data)
  }
  .bss : {
    *(.bss)
  }
}

OUTPUT(boot.img)

