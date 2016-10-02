
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x40020000
#define RCCBASE 0x40023800

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x30);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x30,ra);

    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<2); //PA1
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    ra|=  1<<2; //PA1
    ra|=  1<<4; //PA2
    ra|=  1<<6; //PA3
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<1); //PA1
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<2; //PA1
    ra|=3<<4; //PA2
    ra|=3<<6; //PA3
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<2); //PA1
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    PUT32(GPIOABASE+0x0C,ra);

    PUT32(GPIOABASE+0x18,(1<<(1+0))|(1<<(2+0))|(1<<(3+0)));

    for(rx=0;rx<3;rx++)
    {
        PUT32(GPIOABASE+0x18,(1<<(1+16)));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18,(1<<(1+0)));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18,(1<<(2+16)));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18,(1<<(2+0)));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18,(1<<(3+16)));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18,(1<<(3+0)));
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
