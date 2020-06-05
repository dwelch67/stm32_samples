
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOBBASE 0x50000400
//#define GPIOCBASE 0x50000800
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
//from the manual:
//ST production value: 0xDFFF E1AA
    PUT32(0x40022020,0xDEFFE1AA);
    while(GET32(0x40022010)&0x10000) continue;
    PUT32(0x40022014,0x00020000);
    while(GET32(0x40022010)&0x10000) continue;

    //blinker

    ra=GET32(RCCBASE+0x34);
    ra|=1<<1; //enable port b
    PUT32(RCCBASE+0x34,ra);

    //moder
    ra=GET32(GPIOBBASE+0x00);
    ra&=~(3<<(8<<1)); //PB8
    ra|= (1<<(8<<1)); //PB8
    PUT32(GPIOBBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOBBASE+0x04);
    ra&=~(1<<8); //PB8
    PUT32(GPIOBBASE+0x04,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOBBASE+0x18, (1<<(8+ 0)) );
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOBBASE+0x18, (1<<(8+16)) );
        for(ra=0;ra<200000;ra++) dummy(ra);
    }

    return(0);
}
