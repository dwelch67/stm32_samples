
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40023800
#define RCC_CR          (RCCBASE+0x00)
#define RCC_CFGR        (RCCBASE+0x08)
#define RCC_AHB1ENR     (RCCBASE+0x30)
#define RCC_APB1ENR     (RCCBASE+0x40)

#define GPIOCBASE 0x40020800
#define GPIOC_MODER     (GPIOCBASE+0x00)
#define GPIOC_OTYPER    (GPIOCBASE+0x04)
#define GPIOC_BSRR      (GPIOCBASE+0x18)

#define TIM5BASE  0x40000C00
#define TIM5_CR1        (TIM5BASE+0x00)
#define TIM5_DIER       (TIM5BASE+0x0C)
#define TIM5_SR         (TIM5BASE+0x10)
#define TIM5_CNT        (TIM5BASE+0x24)
#define TIM5_PSC        (TIM5BASE+0x24)
#define TIM5_ARR        (TIM5BASE+0x2C)


//------------------------------------------------------------------------
static void led_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<2; //enable GPIOC
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(GPIOC_MODER);
    ra&=~(3<<(13<<1)); //PC13
    ra|= (1<<(13<<1)); //PC13
    PUT32(GPIOC_MODER,ra);

    ra=GET32(GPIOC_OTYPER);
    ra&=~(1<<13); //PC13
    PUT32(GPIOC_OTYPER,ra);
}
//------------------------------------------------------------------------
static void led_on ( void )
{
    PUT32(GPIOC_BSRR,((1<<13)<<16));
}
//------------------------------------------------------------------------
static void led_off ( void )
{
    PUT32(GPIOC_BSRR,((1<<13)<< 0));
}
//------------------------------------------------------------------------
static void timer_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_APB1ENR);
    ra|=1<<3; //enable TIM5
    PUT32(RCC_APB1ENR,ra);

    PUT32(TIM5_CR1,0x0000);
    PUT32(TIM5_DIER,0x0000);
    PUT32(TIM5_PSC,0x0000);
    PUT32(TIM5_ARR,4000000-1);
    PUT32(TIM5_CNT,4000000-1);
    PUT32(TIM5_CR1,0x0001);
}
//------------------------------------------------------------------------
void do_delay ( unsigned int x )
{
    unsigned int ra;

    for(ra=0;ra<x;ra++)
    {
        while(1)
        {
            if(GET32(TIM5_SR)&1) break;
        }
        PUT32(TIM5_SR,0);
    }
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    led_init();

    timer_init();

    for(rx=0;rx<5;rx++)
    {
        led_on();
        do_delay(1);
        led_off();
        do_delay(1);
    }

    //switch to external clock.
    ra=GET32(RCC_CR);
    ra|=1<<16;
    PUT32(RCC_CR,ra);
    while(1) if(GET32(RCC_CR)&(1<<17)) break;
    ra=GET32(RCC_CFGR);
    ra&=~3;
    ra|=1;
    PUT32(RCC_CFGR,ra);
    while(1) if(((GET32(RCC_CFGR)>>2)&3)==1) break;

//    while(1)
    for(rx=0;rx<5;rx++)
    {
        led_on();
        do_delay(1);
        led_off();
        do_delay(1);
    }

    while(1)
    {
        led_on();
        do_delay(100);
        led_off();
        do_delay(1);
    }

    return(0);
}
