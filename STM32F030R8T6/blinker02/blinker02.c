
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40021000

#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400
#define GPIOCBASE 0x48000800
#define GPIODBASE 0x48000C00


#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#include "mask.h"


int delay ( unsigned int n )
{
    unsigned int ra;

    while(n--)
    {
        while(1)
        {
            ra=GET32(STK_CSR);
            if(ra&(1<<16)) break;
        }
    }
    return(0);
}


int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    ra|=1<<18; //enable port b
    ra|=1<<19; //enable port c
    ra|=1<<20; //enable port d
    PUT32(RCCBASE+0x14,ra);

    PUT32(GPIOABASE+0x00,0x68015555);
    PUT32(GPIOABASE+0x04,0x00000000);
    PUT32(GPIOABASE+0x08,0xCC0003FF);
    PUT32(GPIOABASE+0x0C,0x24000000);

    PUT32(GPIOBBASE+0x00,0x55555555);
    PUT32(GPIOBBASE+0x04,0x00000000);
    PUT32(GPIOBBASE+0x08,0xFFFFFFFF);
    PUT32(GPIOBBASE+0x0C,0x00000000);

    PUT32(GPIOCBASE+0x00,0x55555555);
    PUT32(GPIOCBASE+0x04,0x00000000);
    PUT32(GPIOCBASE+0x08,0xFFFFFFFF);
    PUT32(GPIOCBASE+0x0C,0x00000000);

    //33221100
    //3210
    PUT32(GPIODBASE+0x00,0x00000010);
    PUT32(GPIODBASE+0x04,0x00000000);
    PUT32(GPIODBASE+0x08,0x00000030);
    PUT32(GPIODBASE+0x0C,0x00000000);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

if(0)
{
    for(rx=0;;rx++)
    {
        PUT32(GPIOABASE+0x18,0x000081FF);
        PUT32(GPIOBBASE+0x18,0x0000FFFF);
        PUT32(GPIOCBASE+0x18,0x0000FFFF);
        PUT32(GPIODBASE+0x18,0x00000004);
        delay(5);
        PUT32(GPIOABASE+0x18,0x81FF0000);
        PUT32(GPIOBBASE+0x18,0xFFFF0000);
        PUT32(GPIOCBASE+0x18,0xFFFF0000);
        PUT32(GPIODBASE+0x18,0x00040000);
        delay(5);
        //for(rb=1;rb<0x100;rb<<=1)
        ////for(rb=1<<3;rb<0x10000;rb<<=1)
        ////for(rb=0x8000;rb;rb>>=1)
        //{
            //PUT32(GPIOABASE+0x18,0x00FF0000);
            //PUT32(GPIOABASE+0x18,rb);
            //delay(5);
        //}
        //PUT32(GPIOBBASE+0x18,0xFFFF0000);
        //PUT32(GPIOCBASE+0x18,0xFFFF0000);
        //PUT32(GPIOABASE+0x18,0x00FF0000);
        //delay(5);
    }
}
if(0)
{
    for(rx=0;;rx++)
    {
        PUT32(GPIOABASE+0x18,0x81FF0000);
        PUT32(GPIOBBASE+0x18,0xFFFF0000);
        PUT32(GPIOCBASE+0x18,0xFFE0001F);
        PUT32(GPIODBASE+0x18,0x00040000);
        delay(5);
        PUT32(GPIOABASE+0x18,0x81FF0000);
        PUT32(GPIOBBASE+0x18,0xFFFF0000);
        PUT32(GPIOCBASE+0x18,0xFFFF0000);
        PUT32(GPIODBASE+0x18,0x00040000);
        delay(5);
        //for(rb=1;rb<0x200;rb<<=1)
        for(rb=1<<4;rb<0x10000;rb<<=1)
        //////for(rb=0x8000;rb;rb>>=1)
        {
            PUT32(GPIOABASE+0x18,0x81FF0000);
            PUT32(GPIOBBASE+0x18,0xFFFF0000);
            PUT32(GPIOCBASE+0x18,0xFFFF0000);
            PUT32(GPIODBASE+0x18,0x00040000);
            PUT32(GPIOCBASE+0x18,rb);
            delay(5);
        }
        PUT32(GPIOABASE+0x18,0x81FF0000);
        PUT32(GPIOBBASE+0x18,0xFFFF0000);
        PUT32(GPIOCBASE+0x18,0xFFFF0000);
        PUT32(GPIODBASE+0x18,0x00040000);
        delay(5);
    }
}
if(1)
{
    rb=0;
    for(rx=0;;rx++)
    {
        if(rx>=10)
        {
            PUT32(GPIOABASE+0x18,zero_off[0]);
            PUT32(GPIOBBASE+0x18,zero_off[1]);
            PUT32(GPIOCBASE+0x18,zero_off[2]);
            PUT32(GPIODBASE+0x18,zero_off[3]);
            delay(3);
            rx=0;
        }
        for(ra=0;ra<4;ra++)
        {
            PUT32(GPIOABASE+0x18,mask[ra][0][rx]);
            PUT32(GPIOBBASE+0x18,mask[ra][1][rx]);
            PUT32(GPIOCBASE+0x18,mask[ra][2][rx]);
            PUT32(GPIODBASE+0x18,mask[ra][3][rx]);
        }
        if(rb&1) PUT32(GPIOABASE+0x18,(1<<7)<<0);
        else     PUT32(GPIOABASE+0x18,(1<<7)<<16);
        rb++;
        delay(5);
    }
}


if(0)
{
    while(1)
    {
        for(rb=0;rb<4;rb++)
        {
            for(rx=0;rx<10;rx++)
            {
                PUT32(GPIOABASE+0x18,0x81FF0000);
                PUT32(GPIOBBASE+0x18,0xFFFF0000);
                PUT32(GPIOCBASE+0x18,0xFFFF0000);
                PUT32(GPIODBASE+0x18,0x00040000);


                PUT32(GPIOABASE+0x18,mask[rb][0][rx]);
                PUT32(GPIOBBASE+0x18,mask[rb][1][rx]);
                PUT32(GPIOCBASE+0x18,mask[rb][2][rx]);
                PUT32(GPIODBASE+0x18,mask[rb][3][rx]);
                delay(1);
            }
        }
    }
}









if(0)
{
    unsigned int dig[4];


    PUT32(STK_CSR,4);
    PUT32(STK_RVR,100000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);


    dig[0]=1;
    dig[1]=2;
    dig[2]=3;
    dig[3]=4;
    for(rx=0;;rx++)
    {
        dig[3]++;
        if(dig[3]>=10) { dig[3]=0; dig[2]++;}
        if(dig[2]>=10) { dig[2]=0; dig[1]++;}
        if(dig[1]>=10) { dig[1]=0; dig[0]++;}
        if(dig[0]>=10) dig[0]=0;

        for(ra=0;ra<4;ra++)
        {
            PUT32(GPIOABASE+0x18,mask[ra][0][dig[ra]]);
            PUT32(GPIOBBASE+0x18,mask[ra][1][dig[ra]]);
            PUT32(GPIOCBASE+0x18,mask[ra][2][dig[ra]]);
            PUT32(GPIODBASE+0x18,mask[ra][3][dig[ra]]);
        }
        delay(1);
    }
}





    return(0);
}
