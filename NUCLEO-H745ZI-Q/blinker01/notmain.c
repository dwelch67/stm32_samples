
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define CPUID 0xE000ED00

#define RCCBASE         0x58024400
#define RCC_AHB4ENR     (RCCBASE+0x0E0)

#define GPIOB_BASE      0x58020400
#define GPIOB_MODER     (GPIOB_BASE+0x00)
#define GPIOB_OTYPER    (GPIOB_BASE+0x04)
#define GPIOB_BSRR      (GPIOB_BASE+0x18)

#define GPIOE_BASE      0x58021000
#define GPIOE_MODER     (GPIOE_BASE+0x00)
#define GPIOE_OTYPER    (GPIOE_BASE+0x04)
#define GPIOE_BSRR      (GPIOE_BASE+0x18)


//PB14
//PE1
//PB0

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(CPUID)&0xFFF0;
    //comment one of these
    //if(ra==0xC270) return(0); //cortex-m7
    if(ra==0xC240) return(0); //cortex-m4


    ra=GET32(RCC_AHB4ENR);
    ra|=1<<1; //GPIOB
    ra|=1<<4; //GPIOE
    PUT32(RCC_AHB4ENR,ra);


    ra=GET32(GPIOB_MODER);
    ra&=(~(3<<(0<<1)));
    ra|=  (1<<(0<<1)) ;
    PUT32(GPIOB_MODER,ra);

    ra=GET32(GPIOB_OTYPER);
    ra&=(~(1<<0));
    PUT32(GPIOB_OTYPER,ra);


    ra=GET32(GPIOB_MODER);
    ra&=(~(3<<(14<<1)));
    ra|=  (1<<(14<<1)) ;
    PUT32(GPIOB_MODER,ra);

    ra=GET32(GPIOB_OTYPER);
    ra&=(~(1<<14));
    PUT32(GPIOB_OTYPER,ra);


    ra=GET32(GPIOE_MODER);
    ra&=(~(3<<(1<<1)));
    ra|=  (1<<(1<<1)) ;
    PUT32(GPIOE_MODER,ra);

    ra=GET32(GPIOE_OTYPER);
    ra&=(~(1<<1));
    PUT32(GPIOE_OTYPER,ra);


    while(1)
    {
        PUT32(GPIOB_BSRR,(1<<(14+ 0)));
        for(ra=0;ra<1000000;ra++) dummy(ra);
        PUT32(GPIOB_BSRR,(1<<(14+16)));

        PUT32(GPIOE_BSRR,(1<<( 1+ 0)));
        for(ra=0;ra<1000000;ra++) dummy(ra);
        PUT32(GPIOE_BSRR,(1<<( 1+16)));

        PUT32(GPIOB_BSRR,(1<<( 0+ 0)));
        for(ra=0;ra<1000000;ra++) dummy(ra);
        PUT32(GPIOB_BSRR,(1<<( 0+16)));

    }

    return(0);
}
