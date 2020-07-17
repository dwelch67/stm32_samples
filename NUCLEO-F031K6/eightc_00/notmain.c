
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#include "xtable.h"

void delay ( void )
{
    unsigned int rb;
    for(rb=0;rb<200;rb++) dummy(rb);
}


int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    unsigned int moder;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(0<<1));
    ra&=~(3<<(1<<1));
    ra&=~(3<<(2<<1));
    ra&=~(3<<(3<<1));
    ra&=~(3<<(4<<1));
    moder=ra;
    PUT32(GPIOABASE+0x00,ra);

    PUT32(GPIOABASE+0x18,0x1F<<16);

    while(1)
    {
        for(ra=0;ra<10;ra++)
        {
            for(rc=0;rc<100;rc++)
            {
                for(rb=0;rb<20;rb++)
                {
                    PUT32(GPIOABASE+0x00,moder|xtable[ra][rb][0]);
                    PUT32(GPIOABASE+0x18,xtable[ra][rb][1]);
                    delay();
                    PUT32(GPIOABASE+0x18,0x1F<<16);
                    PUT32(GPIOABASE+0x00,moder);
                }
            }
        }
    }
    return(0);
}
