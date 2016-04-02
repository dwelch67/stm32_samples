
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#include "data.h"

static void quicky ( unsigned int moder, unsigned char o, unsigned char m )
{
    unsigned int ra;
    PUT32(GPIOABASE+0x14,o);
    PUT32(GPIOABASE+0x00,moder|m);
    while(1)
    {
        ra=GET32(STK_CSR);
        if(ra&(1<<16)) break;
    }
    PUT32(GPIOABASE+0x00,moder);
}

#define THREELEDS 10


int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int rz;
    unsigned int moder;



    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    //ra|=1<<(0<<1); //PA0
    //ra|=1<<(1<<1); //PA1
    //ra|=1<<(2<<1); //PA2
    //ra|=1<<(3<<1); //PA3
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<(0<<1); //PA0
    ra|=3<<(1<<1); //PA1
    ra|=3<<(2<<1); //PA2
    ra|=3<<(3<<1); //PA3
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    PUT32(GPIOABASE+0x0C,ra);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,10000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);
    moder=GET32(GPIOABASE+0x00);

    while(1)
    {
        for(ra=0;ra<10;ra++)
        {
            for(rb=0;rb<32;)
            {
                rc=eight[rb++];
                rd=eight[rb++];
                for(rz=0;rz<THREELEDS;rz++)
                {
                    quicky(moder,(rc>>16)&0xFF,(rd>>16)&0xFF);
                    quicky(moder,(rc>> 8)&0xFF,(rd>> 8)&0xFF);
                    quicky(moder,(rc>> 0)&0xFF,(rd>> 0)&0xFF);
                }
            }
        }
        for(ra=0;ra<10;ra++)
        {
            for(rb=0;rb<20;)
            {
                rc=circle[rb++];
                rd=circle[rb++];
                for(rz=0;rz<THREELEDS;rz++)
                {
                    quicky(moder,(rc>>16)&0xFF,(rd>>16)&0xFF);
                    quicky(moder,(rc>> 8)&0xFF,(rd>> 8)&0xFF);
                    quicky(moder,(rc>> 0)&0xFF,(rd>> 0)&0xFF);
                }
            }
        }
    }
    return(0);
}
