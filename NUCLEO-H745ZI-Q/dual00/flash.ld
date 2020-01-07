
MEMORY
{
    rom : ORIGIN = 0x08000000, LENGTH = 0x1000
    ram : ORIGIN = 0x30000000, LENGTH = 0x1000
}
SECTIONS
{
    .text : { *(.text*) } > rom
    .rodata : { *(.rodata*) } > rom
    .bss : { *(.bss*) } > ram
}

