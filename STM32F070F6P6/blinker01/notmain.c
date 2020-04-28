
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE   0x48000000
#define RCCBASE     0x40021000

void notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(0<<1)); //PA0
    ra|= (1<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    ra|= (1<<(1<<1)); //PA1
    ra&=~(3<<(2<<1)); //PA2
    ra|= (1<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra|= (1<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
    ra|= (1<<(4<<1)); //PA4
    ra&=~(3<<(5<<1)); //PA5
    ra|= (1<<(5<<1)); //PA5
    ra&=~(3<<(6<<1)); //PA6
    ra|= (1<<(6<<1)); //PA6
    ra&=~(3<<(7<<1)); //PA7
    ra|= (1<<(7<<1)); //PA7
    PUT32(GPIOABASE+0x00,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOABASE+0x18,0x000000FF);
        for(ra=0;ra<80000;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18,0x00FF0000);
        for(ra=0;ra<80000;ra++) dummy(ra);
    }
}
