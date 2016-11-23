
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40021000

#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400
#define GPIOCBASE 0x48000800
#define GPIODBASE 0x48000C00

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    ra|=1<<18; //enable port b
    ra|=1<<19; //enable port c
    PUT32(RCCBASE+0x14,ra);
 

    ////moder
    //ra=GET32(GPIOABASE+0x00);
    //ra&=~(3<<8); //PA4
    //ra&=~(3<<10); //PA5
    //ra|=1<<8; //PA4
    //ra|=1<<10; //PA5
    //PUT32(GPIOABASE+0x00,ra);
    ////OTYPER
    //ra=GET32(GPIOABASE+0x04);
    //ra&=~(1<<4); //PA4
    //ra&=~(1<<5); //PA5
    //PUT32(GPIOABASE+0x04,ra);
    ////ospeedr
    //ra=GET32(GPIOABASE+0x08);
    //ra|=3<<8; //PA4
    //ra|=3<<10; //PA5
    //PUT32(GPIOABASE+0x08,ra);
    ////pupdr
    //ra=GET32(GPIOABASE+0x0C);
    //ra&=~(3<<8); //PA4
    //ra&=~(3<<10); //PA5
    //PUT32(GPIOABASE+0x0C,ra);


    PUT32(GPIOABASE+0x00,0x28000055);
    PUT32(GPIOABASE+0x04,0x00000000);
    PUT32(GPIOABASE+0x08,0x0C0000FF);
    PUT32(GPIOABASE+0x0C,0x24000000);

    PUT32(GPIOBBASE+0x00,0x55555555);
    PUT32(GPIOBBASE+0x04,0x00000000);
    PUT32(GPIOBBASE+0x08,0xFFFFFFFF);
    PUT32(GPIOBBASE+0x0C,0x00000000);
    
    PUT32(GPIOCBASE+0x00,0x55555555);
    PUT32(GPIOCBASE+0x04,0x00000000);
    PUT32(GPIOCBASE+0x08,0xFFFFFFFF);
    PUT32(GPIOCBASE+0x0C,0x00000000);


    for(rx=0;;rx++)
    {
        PUT32(GPIOBBASE+0x18,0x0000FFFF);
        PUT32(GPIOCBASE+0x18,0xFFFF0000);
        PUT32(GPIOABASE+0x18,0x00FF0000);
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOBBASE+0x18,0xFFFF0000);
        PUT32(GPIOCBASE+0x18,0x0000FFFF);
        PUT32(GPIOABASE+0x18,0x00FF0000);
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOBBASE+0x18,0xFFFF0000);
        PUT32(GPIOCBASE+0x18,0xFFFF0000);
        PUT32(GPIOABASE+0x18,0x000000FF);
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
