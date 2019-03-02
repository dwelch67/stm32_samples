
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOCBASE 0x50000800
#define RCCBASE 0x40021000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    //enable BOOT0 pin

    while(GET32(0x40022010)&0x10000) continue;
    PUT32(0x40022008,0x45670123);
    PUT32(0x40022008,0xCDEF89AB);
    PUT32(0x4002200C,0x08192A3B);
    PUT32(0x4002200C,0x4C5D6E7F);
    PUT32(0x40022020,0xfefffeaa);
    while(GET32(0x40022010)&0x10000) continue;
    PUT32(0x40022014,0x00020000);
    while(GET32(0x40022010)&0x10000) continue;


    //blinker

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
