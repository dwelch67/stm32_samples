
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOCBASE 0x50000800

#define RCCBASE 0x40021000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x34);
    ra|=1<<2; //enable port c
    PUT32(RCCBASE+0x34,ra);

    //moder
    ra=GET32(GPIOCBASE+0x00);
    ra&=~(3<<(6<<1)); //PC6
    ra&=~(3<<(7<<1)); //PC7
    ra|= (1<<(6<<1)); //PC6
    ra|= (1<<(7<<1)); //PC7
    PUT32(GPIOCBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOCBASE+0x04);
    ra&=~(1<<6); //PC6
    ra&=~(1<<7); //PC7
    PUT32(GPIOCBASE+0x04,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOCBASE+0x18, (1<<(6+ 0)) | (1<<(7+16)) );
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOCBASE+0x18, (1<<(6+16)) | (1<<(7+ 0)) );
        for(ra=0;ra<200000;ra++) dummy(ra);
    }

    return(0);
}
