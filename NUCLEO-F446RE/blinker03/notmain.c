
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE         0x40023800
#define RCC_AHB1ENR     (RCCBASE+0x30)
#define RCC_APB1ENR     (RCCBASE+0x40)

#define GPIOABASE       0x40020000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_BSRR      (GPIOABASE+0x18)

#define TIM5BASE        0x40000C00
#define TIM5_CR1        (TIM5BASE+0x00)
#define TIM5_DIER       (TIM5BASE+0x0C)
#define TIM5_SR         (TIM5BASE+0x10)
#define TIM5_CNT        (TIM5BASE+0x24)
#define TIM5_PSC        (TIM5BASE+0x24)
#define TIM5_ARR        (TIM5BASE+0x2C)

//PA5

static void led_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<0; //enable GPIOA
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(5<<1)); //PA5
    ra|= (1<<(5<<1)); //PA5
    PUT32(GPIOA_MODER,ra);
}

static void led_on ( void )
{
    PUT32(GPIOA_BSRR,((1<<5)<< 0));
}

static void led_off ( void )
{
    PUT32(GPIOA_BSRR,((1<<5)<<16));
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
