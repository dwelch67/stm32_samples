
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOB_BASE 0x48000400
#define RCC_BASE 0x40021000

#define TIMER14BASE 0x40002000

void delay ( unsigned int n )
{
    unsigned int ra;

    while(n--)
    {
        while(1)
        {
            ra=GET32(TIMER14BASE+0x24);
            if((ra&(1<<15))!=0) break;
        }
        while(1)
        {
            ra=GET32(TIMER14BASE+0x24);
            if((ra&(1<<15))==0) break;
        }
    }
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


    ra=GET32(RCC_BASE+0x1c);
    ra|=1<<8; //enable timer 14
    PUT32(RCC_BASE+0x1C,ra);

    PUT32(TIMER14BASE+0x2C,0xFFFF);
    PUT32(TIMER14BASE+0x24,0x0000);
    PUT32(TIMER14BASE+0x00,0x0001);

    for(rx=0;;rx++)
    {
        PUT32(GPIOB_BASE+0x18,(1<<(1+16)));
        delay(1000);
        PUT32(GPIOB_BASE+0x18,(1<<1));
        delay(1000);
    }
    return(0);
}
