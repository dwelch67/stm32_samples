
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

#define TIM5BASE  0x40000C00
#define TIM5_CR1        (TIM5BASE+0x00)
#define TIM5_DIER       (TIM5BASE+0x0C)
#define TIM5_SR         (TIM5BASE+0x10)
#define TIM5_CNT        (TIM5BASE+0x24)
#define TIM5_PSC        (TIM5BASE+0x24)
#define TIM5_ARR        (TIM5BASE+0x2C)

#define GPIOBBASE 0x40020400
#define GPIOB_MODER     (GPIOBBASE+0x00)
//#define GPIOB_OTYPER    (GPIOBBASE+0x04)
#define GPIOB_BSRR      (GPIOBBASE+0x18)

//PB0
//PB7
//PB14

static void led_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<1; //enable GPIOB
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(GPIOB_MODER);
    ra&=~(3<<(0<<1)); //PB0
    ra|= (1<<(0<<1)); //PB0
    ra&=~(3<<(7<<1)); //PB7
    ra|= (1<<(7<<1)); //PB7
    ra&=~(3<<(14<<1)); //PB14
    ra|= (1<<(14<<1)); //PB14
    PUT32(GPIOB_MODER,ra);
}

static void led_on ( void )
{
    PUT32(GPIOB_BSRR,((1<<0)<< 0)|((1<<7)<< 0)|((1<<14)<< 0));
}

static void led_off ( void )
{
    PUT32(GPIOB_BSRR,((1<<0)<<16)|((1<<7)<<16)|((1<<14)<<16));
}

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

    //switch to external 8MHz clock.
    ra=GET32(RCC_CR);
    ra|=1<<16;
    PUT32(RCC_CR,ra);
    while(1) if(GET32(RCC_CR)&(1<<17)) break;
    ra=GET32(RCC_CFGR);
    ra&=~3;
    ra|=1;
    PUT32(RCC_CFGR,ra);
    while(1) if(((GET32(RCC_CFGR)>>2)&3)==1) break;

    while(1)
    {
        led_on();
        do_delay(1);
        led_off();
        do_delay(1);
    }
    return(0);
}
