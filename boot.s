.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stackBottom:
.skip 16384
stackTop:

.section .text
.global _start
.type _start, @function
_start:
  mov $stackTop, %esp
	call kernelInit 
  hlt
.size _start, . - _start

