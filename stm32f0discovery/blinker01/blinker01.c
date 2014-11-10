
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOCBASE 0x48000800
#define RCCBASE 0x40021000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<19; //enable port C
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOCBASE+0x00);
    ra&=~(3<<16); //PC8
    ra&=~(3<<18); //PC9
    ra|=1<<16; //PC8
    ra|=1<<18; //PC9
    PUT32(GPIOCBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOCBASE+0x04);
    ra&=~(1<<8); //PC8
    ra&=~(1<<9); //PC9
    PUT32(GPIOCBASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOCBASE+0x08);
    ra|=3<<16; //PC8
    ra|=3<<18; //PC8
    PUT32(GPIOCBASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOCBASE+0x0C);
    ra&=~(3<<16); //PC8
    ra&=~(3<<18); //PC9
    PUT32(GPIOCBASE+0x0C,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOCBASE+0x18,0x01000200);
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOCBASE+0x18,0x02000100);
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
