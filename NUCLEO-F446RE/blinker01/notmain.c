
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE         0x40023800
#define RCC_AHB1ENR     (RCCBASE+0x30)

#define GPIOABASE       0x40020000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_BSRR      (GPIOABASE+0x18)

//PA5

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<0; //enable GPIOA
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(5<<1)); //PA5
    ra|= (1<<(5<<1)); //PA5
    PUT32(GPIOA_MODER,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOA_BSRR,((1<<5)<< 0));
        for(ra=0;ra<800000;ra++) dummy(ra);
        PUT32(GPIOA_BSRR,((1<<5)<<16));
        for(ra=0;ra<800000;ra++) dummy(ra);
    }
    return(0);
}
