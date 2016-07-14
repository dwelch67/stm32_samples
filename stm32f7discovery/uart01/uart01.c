
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x40020000
#define GPIOIBASE 0x40022000
#define RCCBASE 0x40023800

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define USART1BASE 0x40011000

int delay ( unsigned int n )
{
    unsigned int ra;

    while(n--)
    {
        while(1)
        {
            ra=GET32(STK_CSR);
            if(ra&(1<<16)) break;
        }
    }
    return(0);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;


    ra=GET32(RCCBASE+0x30);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x30,ra);

    ra=GET32(RCCBASE+0x44);
    ra|=1<<4; //enable USART1
    PUT32(RCCBASE+0x44,ra);

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
    //afr 0111
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x7<<4; //PA9
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x24);
    ra|=1<<4; //reset USART1
    PUT32(RCCBASE+0x24,ra);
    ra&=~(1<<4);
    PUT32(RCCBASE+0x24,ra);

    PUT32(USART1BASE+0x0C,139);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|1);

    for(rx=0;;rx++)
    {
        while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
        PUT32(USART1BASE+0x28,0x30+(rx&7));
    }


    return(0);
}
