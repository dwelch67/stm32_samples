
MEMORY
{
    ram : ORIGIN = 0x20000000, LENGTH = 0x4000
}

SECTIONS
{
    .text : { *(.text*) } > ram
    .rodata : { *(.rodata*) } > ram
    .bss : { *(.bss*) } > ram
}
