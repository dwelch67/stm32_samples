
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40021000
#define GPIOABASE 0x48000000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port A
    PUT32(RCCBASE+0x14,ra);

    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<10); //PA5
    ra|=1<<10; //PA5
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<5); //PA5
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<10; //PA5
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<10); //PA5
    PUT32(GPIOABASE+0x0C,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOABASE+0x18,((1<<5)<<0));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18,((1<<5)<<16));
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
