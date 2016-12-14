
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x50000000
#define GPIOBBASE 0x50000400

#define RCCBASE 0x40021000

#define FLASHBASE 0x40022000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    //SLOOOOW...
    //ra=GET32(RCCBASE+0x04);
    //ra&=~(7<<13);
    //PUT32(RCCBASE+0x04,ra);

    //shut down flash
    PUT32(FLASHBASE+0x08,0x04152637);
    PUT32(FLASHBASE+0x08,0xFAFBFCFD);
    PUT32(FLASHBASE+0x00,0x00000018);

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<1; //enable port b
    PUT32(RCCBASE+0x2C,ra);
    //moder
    ra=GET32(GPIOBBASE+0x00);
    ra&=~(3<<2); //PB1
    ra|=  1<<2; //PB1
    PUT32(GPIOBBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOBBASE+0x04);
    ra&=~(1<<1); //PB1
    PUT32(GPIOBBASE+0x04,ra);

    for(rx=0;;rx++)
    {

        PUT32(GPIOBBASE+0x18, (1<<(1+16)) );

        //PUT32(GPIOABASE+0x18, (1<<(4+ 0)) | (1<<(5+16)) );
        for(ra=0;ra<10;ra++) dummy(ra);

        //PUT32(GPIOBBASE+0x18, (1<<(1+0)) );

        //PUT32(GPIOABASE+0x18, (1<<(4+16)) | (1<<(5+16)) );
        for(ra=0;ra<4000;ra++) dummy(ra);

        PUT32(GPIOBBASE+0x18, (1<<(1+0)) );

        //PUT32(GPIOABASE+0x18, (1<<(4+16)) | (1<<(5+ 0)) );
        for(ra=0;ra<10;ra++) dummy(ra);
        //PUT32(GPIOABASE+0x18, (1<<(4+16)) | (1<<(5+16)) );
        for(ra=0;ra<4000;ra++) dummy(ra);
    }

    return(0);
}
