
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#define TIMER14BASE 0x40002000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    //ra=GET32(RCCBASE+0x14);
    //ra|=1<<17; //enable port a
    //PUT32(RCCBASE+0x14,ra);
    ////moder
    //ra=GET32(GPIOABASE+0x00);
    //ra&=~(3<<8); //PA4
    //ra|=(2<<8); //PA4 alternate function
    //PUT32(GPIOABASE+0x00,ra);
    ////AFRL
    //ra=GET32(GPIOABASE+0x20);
    //ra&=~(0xF<<16); //PA4
    //ra|=4<<16; //PA4
    //PUT32(GPIOABASE+0x20,ra);



    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<10); //PA5
    ra|=1<<10; //PA5
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<5); //PA5
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<10; //PA5
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<10); //PA5
    PUT32(GPIOABASE+0x0C,ra);

    ra=GET32(RCCBASE+0x1c);
    ra|=1<<8; //enable timer 14
    PUT32(RCCBASE+0x1C,ra);
    
    PUT32(TIMER14BASE+0x2C,0xFFFF);
    PUT32(TIMER14BASE+0x24,0x0000);
    PUT32(TIMER14BASE+0x28,0x0000);
    PUT32(TIMER14BASE+0x00,0x0001);

    for(rx=0;;rx++)
    {
        PUT32(GPIOABASE+0x18,(1<<(5+16)));
        while(1)
        {
            if((GET32(TIMER14BASE)&0x8000)!=0) break;
        }
        PUT32(GPIOABASE+0x18,(1<<(5+0)));
        while(1)
        {
            if((GET32(TIMER14BASE)&0x8000)==0) break;
        }
    }


    return(0);
}
