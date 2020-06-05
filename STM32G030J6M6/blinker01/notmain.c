
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE   0x50000000
#define RCCBASE     0x40021000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x34);
    ra|=1<<0; //enable port A
    PUT32(RCCBASE+0x34,ra);

    //MODER
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(13<<1)); //PB13
    ra|= (1<<(13<<1)); //PB13
    PUT32(GPIOABASE+0x00,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOABASE+0x18, (1<<(13+ 0)) );
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18, (1<<(13+16)) );
        for(ra=0;ra<200000;ra++) dummy(ra);
    }

    return(0);
}
