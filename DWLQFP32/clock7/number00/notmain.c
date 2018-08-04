
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );


#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400
#define RCC_BASE 0x40021000

#include "seven.h"

void delay ( void )
{
    unsigned int ra;
    for(ra=0;ra<0x10000;ra++) dummy(ra);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;


    ra=GET32(RCC_BASE+0x14);
    ra|=1<<17; //enable port A
    ra|=1<<18; //enable port B
    PUT32(RCC_BASE+0x14,ra);




    //moder
    PUT32(GPIOABASE+0x00,0x28005555);
    //OTYPER
    PUT32(GPIOABASE+0x04,0x00000000);
    //BSRR
    PUT32(GPIOABASE+0x18,0x00FF0000);

    //moder
    PUT32(GPIOBBASE+0x00,0x00000000);
    //OTYPER
    PUT32(GPIOBBASE+0x04,0x00000000);
    //BSRR
    PUT32(GPIOBBASE+0x18,0x00F00000);

    while(1)
    {
        for(ra=0;ra<10;ra++)
        {
            PUT32(GPIOABASE+0x18,seven[ra]);
            for(rx=4;rx<8;rx++)
            {
                PUT32(GPIOBBASE+0x00,(1<<(rx<<1)));
                delay();
            }
        }
    }
    return(0);
}


/*

reset halt
flash write_image erase number00/notmain.elf
reset

*/


