
MEMORY
{
    rom : ORIGIN = 0x08100000, LENGTH = 0x1000
    ram : ORIGIN = 0x30020000, LENGTH = 0x1000
}
SECTIONS
{
    .text : { *(.text*) } > rom
    .rodata : { *(.rodata*) } > rom
    .bss : { *(.bss*) } > ram
}

