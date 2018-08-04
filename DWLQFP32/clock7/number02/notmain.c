
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400
#define RCC_BASE 0x40021000

#include "seven.h"

static unsigned int num[4];

void delay ( void )
{
    unsigned int ra;
    for(ra=0;ra<0x1000;ra++) dummy(ra);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
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

    num[0]=1;
    num[1]=2;
    num[2]=3;
    num[3]=4;

    while(1)
    {
        for(rb=0;rb<10;rb++)
        {
            for(rx=0;rx<4;rx++)
            {
                PUT32(GPIOBBASE+0x00,0x00000000);
                PUT32(GPIOABASE+0x18,seven[num[rx]]);
                PUT32(GPIOBBASE+0x00,(1<<((rx+4)<<1)));
                delay();
            }
        }
        num[3]++;
        if(num[3]>9) { num[3]=0; num[2]++; }
        if(num[2]>9) { num[2]=0; num[1]++; }
        if(num[1]>9) { num[1]=0; num[0]++; }
        if(num[0]>9) { num[0]=0;  }
    }
    return(0);
}

/*

reset halt
flash write_image erase number02/notmain.elf
reset

*/


