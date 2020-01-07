
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
#define RCCBASE 0x58024400
#define RCC_APB4ENR (RCCBASE+0F4)
#define RCC_C1_APB4ENR (RCCBASE+154)
#define RCC_C2_APB4ENR (RCCBASE+1B4)
#define SYSCFG_BASE 0x58000400
#define RCC_AHB4ENR (RCCBASE+0x0E0)
#define CPUID 0xE000ED00

#define GPIOB_BASE      0x58020400
#define GPIOB_MODER     (GPIOB_BASE+0x00)
#define GPIOB_OTYPER    (GPIOB_BASE+0x04)
#define GPIOB_BSRR      (GPIOB_BASE+0x18)


//PB14

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(CPUID);
    if((ra&0xFFF0)==0xC270)
    {
        PUT32(0x20000000,ra);
    }
    else
    {
        PUT32(0x20000004,ra);
        return(0);
    }

//    ra=GET32(RCC_APB4ENR);
    //ra|=1<<1;
    //PUT32(RCC_APB4ENR,ra);
    ra=GET32(RCC_AHB4ENR);
    ra|=1<<1;
    PUT32(RCC_AHB4ENR,ra);

    ra=GET32(GPIOB_MODER);
    ra&=(~(3<<(14<<1)));
    ra|=  (1<<(14<<1)) ;
    PUT32(GPIOB_MODER,ra);

    ra=GET32(GPIOB_OTYPER);
    ra&=(~(1<<14));
    PUT32(GPIOB_OTYPER,ra);

    while(1)
    {
        PUT32(GPIOB_BSRR,1 << (14+ 0));
        for(ra=0;ra<1000000;ra++) dummy(ra);
        PUT32(GPIOB_BSRR,1 << (14+16));
        for(ra=0;ra<1000000;ra++) dummy(ra);
    }

    return(0);
}
