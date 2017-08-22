
MEMORY
{
    rom : ORIGIN = 0x08000000, LENGTH = 0x1000
}

SECTIONS
{
    .text : { *(.text*) } > rom
    .rodata : { *(.rodata*) } > rom
}

