
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define RCCBASE 0x40023800
#define RCC_AHB1ENR (RCCBASE+0x30)
#define RCC_APB1ENR (RCCBASE+0x40)

#define GPIOABASE 0x40020000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_OTYPER    (GPIOABASE+0x04)
#define GPIOA_OSPEEDR   (GPIOABASE+0x08)
#define GPIOA_PUPDR     (GPIOABASE+0x0C)
#define GPIOA_BSRR      (GPIOABASE+0x18)

#define GPIOBBASE 0x40020400
#define GPIOB_MODER     (GPIOBBASE+0x00)
#define GPIOB_OTYPER    (GPIOBBASE+0x04)
#define GPIOB_BSRR      (GPIOBBASE+0x18)

#define TIM5BASE  0x40000C00
#define TIM5_CR1        (TIM5BASE+0x00)
#define TIM5_DIER       (TIM5BASE+0x0C)
#define TIM5_SR         (TIM5BASE+0x10)
#define TIM5_CNT        (TIM5BASE+0x24)
#define TIM5_PSC        (TIM5BASE+0x24)
#define TIM5_ARR        (TIM5BASE+0x2C)

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
    PUT32(TIM5_ARR,16000000-1);
    PUT32(TIM5_CNT,16000000-1);
    PUT32(TIM5_CR1,0x0001);
}

static void do_delay ( unsigned int x )
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
    unsigned int rx;

    led_init();
    timer_init();

    for(rx=0;rx<5;rx++)
    {
        led_on();
        while(1) if((GET32(TIM5_CNT)&0x200000)!=0) break;
        led_off();
        while(1) if((GET32(TIM5_CNT)&0x200000)==0) break;
    }
    while(1)
    {
        led_on();
        do_delay(10);
        led_off();
        do_delay(1);
    }
    return(0);
}
