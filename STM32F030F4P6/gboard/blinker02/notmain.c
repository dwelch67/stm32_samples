
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400
#define GPIOCBASE 0x48000800

#define RCCBASE 0x40021000

#include "gbinary.h"

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;
    unsigned int ry;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    PUT32(GPIOABASE+0x00,0x28000000);
    //OTYPER
    PUT32(GPIOABASE+0x04,0x00000000);
    //BSRR
    PUT32(GPIOABASE+0x18,0x00FF0000);

    while(1)
    {
        for(ry=0;ry<3;ry++)
        {
            for(rx=0;rx<4;rx++)
            {
                PUT32(GPIOABASE+0x00,0x28200000);
                PUT32(GPIOABASE+0x18,0x00FF0000);
                PUT32(GPIOABASE+0x00,gbinary[ry][rx][0]);
                PUT32(GPIOABASE+0x18,gbinary[ry][rx][1]);
                for(ra=0;ra<200000;ra++) dummy(ra);
            }
        }
    }
    return(0);
}
/*

halt
flash write_image erase gboard/blinker02/notmain.elf
reset

*/
