
MEMORY
{
    /* rom : ORIGIN = 0x08000000, LENGTH = 0x1000 *//*AXIM*/
    rom : ORIGIN = 0x00200000, LENGTH = 0x1000 /*ITCM*/
    ram : ORIGIN = 0x20000000, LENGTH = 0x1000
}
SECTIONS
{
    .text   : { *(.text*)   } > rom
    .rodata : { *(.rodata*) } > rom
    .bss    : { *(.bss*)    } > ram
}

