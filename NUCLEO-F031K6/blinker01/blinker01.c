
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOBBASE 0x48000400
#define RCCBASE 0x40021000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<18; //enable port b
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOBBASE+0x00);
    ra&=~(3<<6); //PB3
    ra|=1<<6; //PB3
    PUT32(GPIOBBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOBBASE+0x04);
    ra&=~(1<<3); //PB3
    PUT32(GPIOBBASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOBBASE+0x08);
    ra|=3<<6; //PB3
    PUT32(GPIOBBASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOBBASE+0x0C);
    ra&=~(3<<6); //PB3
    PUT32(GPIOBBASE+0x0C,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOBBASE+0x18,(1<<3));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOBBASE+0x18,(1<<(3+16)));
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
