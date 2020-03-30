
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOA_BASE       0x48000000
#define GPIOA_MODER     (GPIOA_BASE+0x00)
#define GPIOA_OTYPER    (GPIOA_BASE+0x04)
#define GPIOA_BSRR      (GPIOA_BASE+0x18)

#define RCC_BASE        0x40021000
#define RCC_AHBENR      (RCC_BASE+0x14)

#define STK_CSR         0xE000E010
#define STK_RVR         0xE000E014
#define STK_CVR         0xE000E018
#define STK_MASK        0x00FFFFFF

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

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port a
    PUT32(RCC_AHBENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
    ra&=~(3<<(5<<1)); //PA5
    ra&=~(3<<(6<<1)); //PA6
    ra&=~(3<<(7<<1)); //PA7
    ra|= (1<<(0<<1)); //PA0
    ra|= (1<<(1<<1)); //PA1
    ra|= (1<<(2<<1)); //PA2
    ra|= (1<<(3<<1)); //PA3
    ra|= (1<<(4<<1)); //PA4
    ra|= (1<<(5<<1)); //PA5
    ra|= (1<<(6<<1)); //PA6
    ra|= (1<<(7<<1)); //PA7
    PUT32(GPIOA_MODER,ra);

    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    ra&=~(1<<4); //PA4
    ra&=~(1<<5); //PA5
    ra&=~(1<<6); //PA6
    ra&=~(1<<7); //PA7
    PUT32(GPIOA_OTYPER,ra);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

    while(1)
    {
        PUT32(GPIOA_BSRR,0x000000FF);
        delay(16);
        PUT32(GPIOA_BSRR,0x00FF0000);
        delay(16);
    }

    return(0);
}
