
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40023800
#define RCC_AHB1ENR (RCCBASE+0x30)

#define GPIOCBASE 0x40020800
#define GPIOC_MODER     (GPIOCBASE+0x00)
#define GPIOC_OTYPER    (GPIOCBASE+0x04)
#define GPIOC_BSRR      (GPIOCBASE+0x18)

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<2; //enable GPIOC
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(GPIOC_MODER);
    ra&=~(3<<(13<<1)); //PC13
    ra|= (1<<(13<<1)); //PC13
    PUT32(GPIOC_MODER,ra);
    //OTYPER
    ra=GET32(GPIOC_OTYPER);
    ra&=~(1<<13); //PC13
    PUT32(GPIOC_OTYPER,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIOC_BSRR,((1<<13)<<0));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOC_BSRR,((1<<13)<<16));
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}

/*

dfu-util -a 0 -s 0x08000000 -D notmain.bin

*/
