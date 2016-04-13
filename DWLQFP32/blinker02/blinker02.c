
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOB_BASE 0x48000400
#define RCC_BASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF


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

    ra=GET32(RCC_BASE+0x14);
    ra|=1<<18; //enable port B
    PUT32(RCC_BASE+0x14,ra);
    //moder
    ra=GET32(GPIOB_BASE+0x00);
    ra&=~(3<<2); //PB1
    ra|=1<<2; //PB1
    PUT32(GPIOB_BASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOB_BASE+0x04);
    ra&=~(1<<1); //PB1
    PUT32(GPIOB_BASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOB_BASE+0x08);
    ra|=3<<2; //PB1
    PUT32(GPIOB_BASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOB_BASE+0x0C);
    ra&=~(3<<2); //PB1
    PUT32(GPIOB_BASE+0x0C,ra);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

    for(rx=0;;rx++)
    {
        PUT32(GPIOB_BASE+0x18,(1<<(1+16)));
        delay(50);
        PUT32(GPIOB_BASE+0x18,(1<<1));
        delay(50);
    }
    return(0);
}
