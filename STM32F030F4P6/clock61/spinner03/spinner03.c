
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000

#define RCCBASE 0x40021000

#define STK_CSR     0xE000E010
#define STK_RVR     0xE000E014
#define STK_CVR     0xE000E018
#define STK_MASK    0x00FFFFFF

#include "twostuff.h"

volatile unsigned int counter;
volatile unsigned int sec;
volatile unsigned int second;
void systick_handler ( void )
{
    if(counter==0)
    {
        sec++;
        if(sec>59) sec=0;
        PUT32(GPIOABASE+0x00,0x28000000);
        PUT32(GPIOABASE+0x18,0x00FF0000);
        PUT32(GPIOABASE+0x00,twostuff[sec][0]);
        PUT32(GPIOABASE+0x18,twostuff[sec][1]);
        
        if(sec==second) counter=10;
        else            counter=1;
        if(sec==15) counter=60;
        if(sec==16) counter=10;
    }
    counter--;
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

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000-1);
    PUT32(STK_CVR,0x00000000);

    second = 0;
    counter = 0;
    sec = 0;
    
    PUT32(STK_CSR,7);

    while(1)
    {
        for(second=0;second<60;second++)
        {
            for(ra=0;ra<20000;ra++) dummy(ra);
        }
    }
    return(0);
}


/*

halt
flash write_image erase clock61/spinner03/spinner03.elf
reset

*/


