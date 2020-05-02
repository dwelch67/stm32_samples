
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40023800
#define RCC_AHB1ENR   (RCCBASE+0x30)
#define RCC_AHB1LPENR (RCCBASE+0x50)

#define GPIOABASE 0x40020000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_OTYPER    (GPIOABASE+0x04)
#define GPIOA_BSRR      (GPIOABASE+0x18)

#define GPIOBBASE 0x40020400
#define GPIOB_MODER     (GPIOBBASE+0x00)
#define GPIOB_OTYPER    (GPIOBBASE+0x04)
#define GPIOB_BSRR      (GPIOBBASE+0x18)

//PB0
//PB7
//PB14

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<1; //enable GPIOB
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(GPIOB_MODER);
    ra&=~(3<<( 0<<1)); //PB0
    ra|= (1<<( 0<<1)); //PB0
    ra&=~(3<<( 7<<1)); //PB7
    ra|= (1<<( 7<<1)); //PB7
    ra&=~(3<<(14<<1)); //PB14
    ra|= (1<<(14<<1)); //PB14
    PUT32(GPIOB_MODER,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOB_BSRR,((1<<0)<<0)|((1<<7)<<16)|((1<<14)<<0));
        for(ra=0;ra<800000;ra++) dummy(ra);
        PUT32(GPIOB_BSRR,((1<<0)<<16)|((1<<7)<<0)|((1<<14)<<16));
        for(ra=0;ra<800000;ra++) dummy(ra);
    }
    return(0);
}
