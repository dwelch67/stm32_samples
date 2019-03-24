
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE   0x50000000
#define RCCBASE     0x40021000

const unsigned int seven_up[10]=
{
0x00880077,
0x00BE0041,
0x001900E6,
0x001C00E3,
0x002E00D1,
0x004C00B3,
0x004800B7,
0x009E0061,
0x000800F7,
0x000C00F3,
};
const unsigned int seven_dn[10]=
{
0x00770088,
0x004100BE,
0x00E60019,
0x00E3001C,
0x00D1002E,
0x00B3004C,
0x00B70048,
0x0061009E,
0x00F70008,
0x00F3000C,
};

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;
    unsigned int ry;

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
    ra|=1<<(0<<1); //PA0
    ra|=1<<(1<<1); //PA1
    ra|=1<<(2<<1); //PA2
    ra|=1<<(3<<1); //PA3
    ra|=1<<(4<<1); //PA4
    ra|=1<<(5<<1); //PA5
    ra|=1<<(6<<1); //PA6
    ra|=1<<(7<<1); //PA7
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

    for(rx=8;;rx++)
    {
        if(rx>9) rx=0;
        for(ry=0;ry<200;ry++)
        {
            PUT32(GPIOABASE+0x18,seven_up[rx]);
            for(ra=0;ra<200;ra++) dummy(ra);
            PUT32(GPIOABASE+0x18,seven_dn[rx]);
            for(ra=0;ra<200;ra++) dummy(ra);
        }
    }
    return(0);
}
