
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOB_BASE 0x48000400
#define RCC_BASE 0x40021000

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

    for(rx=0;;rx++)
    {
        PUT32(GPIOB_BASE+0x18,(1<<1));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOB_BASE+0x18,(1<<(1+16)));
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
