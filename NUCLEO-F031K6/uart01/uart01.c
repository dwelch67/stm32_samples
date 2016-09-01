
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000
#define USART1BASE 0x40013800

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x18,ra);

    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<4); //PA2
    ra|=2<<4; //PA2
    ra&=~(3<<18); //PA9
    ra|=2<<18; //PA9
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER 0
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<2); //PA2
    ra&=~(1<<9); //PA9
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr 11
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<4; //PA2
    ra|=3<<18; //PA9
    PUT32(GPIOABASE+0x08,ra);
    //pupdr 00
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<4); //PA2
    ra&=~(3<<18); //PA9
    PUT32(GPIOABASE+0x0C,ra);
    //afr 0001
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<8); //PA2
    ra|=0x1<<8; //PA2
    PUT32(GPIOABASE+0x20,ra);
    //afr 0001
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x1<<4; //PA9
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x0C);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x0C,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x0C,ra);

    PUT32(USART1BASE+0x0C,69);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|1);

    for(rx=0;;rx++)
    {
        while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
        PUT32(USART1BASE+0x28,0x30+(rx&7));
    }
    return(0);
}
