
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x50000000
#define GPIOBBASE 0x50000400

#define RCCBASE 0x40021000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x2C,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
    ra&=~(3<<(5<<1)); //PA5
    ra&=~(3<<(6<<1)); //PA6
    ra&=~(3<<(7<<1)); //PA7
    ra|= (1<<(0<<1)); //PA0
    ra|= (1<<(1<<1)); //PA1
    ra|= (1<<(2<<1)); //PA2
    ra|= (1<<(3<<1)); //PA3
    ra|= (1<<(4<<1)); //PA4
    ra|= (1<<(5<<1)); //PA5
    ra|= (1<<(6<<1)); //PA6
    ra|= (1<<(7<<1)); //PA7
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    ra&=~(1<<4); //PA4
    ra&=~(1<<5); //PA5
    ra&=~(1<<6); //PA6
    ra&=~(1<<7); //PA7
    PUT32(GPIOABASE+0x04,ra);

    for(rx=0;;rx++)
    {
        ra=rx;
        ra|=((~rx)&0xFF)<<16;
        PUT32(GPIOABASE+0x18,ra);
        for(ra=0;ra<20000;ra++) dummy(ra);
    }

    return(0);
}
