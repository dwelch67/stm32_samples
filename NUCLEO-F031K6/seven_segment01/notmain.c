
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40021000
#define GPIOABASE   0x48000000

const unsigned int seven[10]=
{
0x002101DA,
0x01AB0050,
0x00620199,
0x012200D9,
0x01A80053,
0x013000CB,
0x003001CB,
0x01A30058,
0x002001DB,
0x01A0005B,
};

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;


    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    //ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
    ra&=~(3<<(5<<1)); //PA5
    ra&=~(3<<(6<<1)); //PA6
    ra&=~(3<<(7<<1)); //PA7
    ra&=~(3<<(8<<1)); //PA7
    ra|=1<<(0<<1); //PA0
    ra|=1<<(1<<1); //PA1
    //ra|=3<<(2<<1); //PA2
    ra|=1<<(3<<1); //PA3
    ra|=1<<(4<<1); //PA4
    ra|=1<<(5<<1); //PA5
    ra|=1<<(6<<1); //PA6
    ra|=1<<(7<<1); //PA7
    ra|=1<<(8<<1); //PA7
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    //ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    ra&=~(1<<4); //PA4
    ra&=~(1<<5); //PA5
    ra&=~(1<<6); //PA6
    ra&=~(1<<7); //PA7
    ra&=~(1<<8); //PA7
    PUT32(GPIOABASE+0x04,ra);

    for(rx=0;;rx++)
    {
        if(rx>9) rx=0;
        PUT32(GPIOABASE+0x18,seven[rx]);
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
