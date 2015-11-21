
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define USART1BASE 0x40013800

#define GPIOABASE 0x50000000
#define GPIOBBASE 0x50000400
#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF


int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;


    ra=GET32(RCCBASE+0x00);
    ra|=1<<0;
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x00);
        if(ra&(1<<2)) break;
    }
    PUT32(RCCBASE+0x0C,1);

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x2C,ra);

    ra=GET32(RCCBASE+0x34);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x34,ra);

    ra=GET32(RCCBASE+0x4C);
    ra&=~(3<<0);
    ra|=2<<0; //HSI16 clock for uart
    PUT32(RCCBASE+0x4C,ra);


    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<18); //PA9
    ra|=2<<18; //PA9
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER 0
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<9); //PA9
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr 11
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<18; //PA9
    PUT32(GPIOABASE+0x08,ra);
    //pupdr 00
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<18); //PA9
    PUT32(GPIOABASE+0x0C,ra);
    //afr4 0100
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x4<<4; //PA9 AF4 USART1_TX
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x24);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x24,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x24,ra);

    PUT32(USART1BASE+0x0C,139);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|1);

    PUT32(USART1BASE+0x28,0x55);
    for(rx=0;;rx++)
    {
        while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
        PUT32(USART1BASE+0x28,0x30+(rx&7));
    }
    return(0);
}
