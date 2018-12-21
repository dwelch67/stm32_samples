
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#include "twostuff.h"

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    PUT32(GPIOABASE+0x00,0x28000000);
    //OTYPER
    PUT32(GPIOABASE+0x04,0x00000000);
    PUT32(GPIOABASE+0x18,0x00FF0000);

    while(1)
    {
        for(rx=0;rx<60;rx++)
        {
            //PUT32(GPIOABASE+0x00,0x00000000);
            //PUT32(GPIOBBASE+0x00,0x00000000);
            //PUT32(GPIOABASE+0x18,0x00FF0000);
            //PUT32(GPIOBBASE+0x18,0x00FF0000);
            PUT32(GPIOABASE+0x00,twostuff[rx][0]);
            PUT32(GPIOABASE+0x18,twostuff[rx][1]);
            for(ra=0;ra<10000;ra++) dummy(ra);
        }
    }


    
    return(0);
}
