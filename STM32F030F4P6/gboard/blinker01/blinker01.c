
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400
#define GPIOCBASE 0x48000800

#define RCCBASE 0x40021000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(4<<1));
    ra|= (1<<(4<<1));
    ra&=~(3<<(5<<1));
    ra|= (1<<(5<<1));
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<4); //PA4
    ra&=~(1<<5); //PA5
    PUT32(GPIOABASE+0x04,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOABASE+0x18, (1<<(4+ 0))|(1<<(5+16)) );
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18, (1<<(4+16))|(1<<(5+ 0)) );
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
/*

halt
flash write_image erase blinker01/two/blinker01.elf
reset

*/
