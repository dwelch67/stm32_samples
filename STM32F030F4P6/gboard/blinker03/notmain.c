
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000

#define RCCBASE 0x40021000

#define STK_CSR     0xE000E010
#define STK_RVR     0xE000E014
#define STK_CVR     0xE000E018
#define STK_MASK    0x00FFFFFF

#include "gbinary.h"

volatile unsigned int hour;
volatile unsigned int mten;
volatile unsigned int msec;
volatile unsigned int counter;
void systick_handler ( void )
{
    unsigned int y;
    unsigned int x;
    unsigned int z;
    y=(counter>>5)&3;
    x=(counter>>3)&3;
    switch(y)
    {
        case 0: z=hour; break;
        case 1: z=mten; break;
        case 2: z=msec; break;
        default: z=0; break;
    }
    switch(counter&7)
    {
        case 0:
        {
            PUT32(GPIOABASE+0x00,0x28200000);
            PUT32(GPIOABASE+0x18,0x00FF0000);
            PUT32(GPIOABASE+0x00,gbinary[y][x][0]);
            PUT32(GPIOABASE+0x18,gbinary[y][x][1]);
            break;
        }
        case 2:
        {
            if((z&(1<<x))==0)
            {
            PUT32(GPIOABASE+0x00,0x28200000);
            PUT32(GPIOABASE+0x18,0x00FF0000);
            }
            break;
        }
        default:
        {
            break;
        }
    }


    



    counter++;
    if(counter>=0x5F) counter=0;
}

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    //moder
    PUT32(GPIOABASE+0x00,0x28000000);
    //OTYPER
    PUT32(GPIOABASE+0x04,0x00000000);
    //BSRR
    PUT32(GPIOABASE+0x18,0x00FF0000);

    hour=0xB;
    mten=3;
    msec=9;
    counter=0;

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,7);

    while(1)
    {
        for(msec=0;msec<10;msec++)
        {
            for(ra=0;ra<200000;ra++) dummy(ra);
        }
    }

    return(0);
}


/*

halt
flash write_image erase gboard/blinker03/notmain.elf
reset

*/


