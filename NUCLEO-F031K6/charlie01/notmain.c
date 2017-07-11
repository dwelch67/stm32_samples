
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40021000
#define GPIOABASE   0x48000000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;


    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    //ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
    ra|=1<<(0<<1); //PA0
    ra|=1<<(1<<1); //PA1
    //ra|=3<<(2<<1); //PA2
    ra|=1<<(3<<1); //PA3
    ra|=1<<(4<<1); //PA4
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    //ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    ra&=~(1<<4); //PA4
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<(0<<1); //PA0
    ra|=3<<(1<<1); //PA1
    //ra|=3<<(2<<1); //PA2
    ra|=3<<(3<<1); //PA3
    ra|=3<<(4<<1); //PA4
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    //ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
    PUT32(GPIOABASE+0x0C,ra);

    for(rx=0;;rx++)
    {
        ra=((rx&0xC)<<1)|(rx&0x3);
        ra=(ra<<16)|((~ra)&0x1B);
        PUT32(GPIOABASE+0x18,ra);
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
