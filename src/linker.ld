OUTPUT_FORMAT(elf32-i386)
ENTRY(kmain)

SECTIONS
{
    . = 0x9000;
    .text : { *(.text.start) *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss) *(COMMON) }
}
