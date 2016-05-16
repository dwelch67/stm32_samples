
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOIBASE 0x40022000
#define RCCBASE 0x40023800

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;


    ra=GET32(RCCBASE+0x30);
    ra|=1<<8; //enable port i
    PUT32(RCCBASE+0x30,ra);
    //moder
    ra=GET32(GPIOIBASE+0x00);
    ra&=~(3<<2); //PI1
    ra|=1<<2; //PI1
    PUT32(GPIOIBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOIBASE+0x04);
    ra&=~(1<<1); //PI1
    PUT32(GPIOIBASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOIBASE+0x08);
    ra|=3<<2; //PI1
    PUT32(GPIOIBASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOIBASE+0x0C);
    ra&=~(3<<2); //PI1
    PUT32(GPIOIBASE+0x0C,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOIBASE+0x18,(1<<(1+0)));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOIBASE+0x18,(1<<(1+16)));
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
