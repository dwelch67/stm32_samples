
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOB_BASE      0x50000400
#define GPIOB_MODER     (GPIOB_BASE+0x00)
#define GPIOB_OTYPER    (GPIOB_BASE+0x04)
#define GPIOB_BSRR      (GPIOB_BASE+0x18)

#define RCC_BASE        0x40021000
#define RCC_CR          (RCC_BASE+0x00)
#define RCC_CFGR        (RCC_BASE+0x0C)
#define RCC_IOPENR      (RCC_BASE+0x2C)

#define STK_CSR         0xE000E010
#define STK_RVR         0xE000E014
#define STK_CVR         0xE000E018
#define STK_MASK        0x00FFFFFF

void clock_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_CR);
    ra|=1; //HSI16ON
    PUT32(RCC_CR,ra);
    while(1)
    {
        ra=GET32(RCC_CR);
        if(ra&(1<<2)) break; //HSI16RDYF
    }
    ra=GET32(RCC_CFGR);
    ra&=(~3);
    ra|=( 1); //HSI16
    PUT32(RCC_CFGR,ra);
    while(1)
    {
        ra=GET32(RCC_CFGR);
        if(((ra>>2)&3)==1) break;
    }
}

void delay ( unsigned int x )
{
    while(x--)
    {
        while(1)
        {
            if(GET32(STK_CSR)&0x10000) break;
        }
    }
}

int notmain ( void )
{
    unsigned int ra;

    clock_init();

    ra=GET32(RCC_IOPENR);
    ra|=1<<1; //GIOB enable
    PUT32(RCC_IOPENR,ra);

    ra=GET32(GPIOB_MODER);
    ra&=~(3<<(5<<1)); //PB5
    ra|= (1<<(5<<1)); //PB5
    PUT32(GPIOB_MODER,ra);

    ra=GET32(GPIOB_OTYPER);
    ra&=~(1<<5); //PB5
    PUT32(GPIOB_OTYPER,ra);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,16000000-1);
    PUT32(STK_CVR,0);
    PUT32(STK_CSR,5);

    while(1)
    {
        PUT32(GPIOB_BSRR, (1<<(5+ 0)) );
        delay(5);
        PUT32(GPIOB_BSRR, (1<<(5+16)) );
        delay(5);
    }
    return(0);
}
