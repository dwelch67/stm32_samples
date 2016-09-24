
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOCBASE 0x40011000
#define RCCBASE 0x40021000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x18);
    ra|=1<<4; //enable port c
    PUT32(RCCBASE+0x18,ra);
    //config
    ra=GET32(GPIOCBASE+0x04);
    ra&=~(3<<20);   //PC13
    ra|=1<<20;      //PC13
    ra&=~(3<<22);   //PC13
    ra|=0<<22;      //PC13
    PUT32(GPIOCBASE+0x04,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOCBASE+0x10,1<<(13+0));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOCBASE+0x10,1<<(13+16));
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
