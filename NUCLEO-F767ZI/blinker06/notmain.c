
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define RCCBASE         0x40023800
#define RCC_CR          (RCCBASE+0x00)
#define RCC_CFGR        (RCCBASE+0x08)
#define RCC_AHB1ENR     (RCCBASE+0x30)
#define RCC_APB1ENR     (RCCBASE+0x40)
#define RCC_BDCR        (RCCBASE+0x70)

#define PWRBASE         0x40007000
#define PWR_CR          (PWRBASE+0x00)

#define RTCBASE         0x40002800
#define RTC_TR          (RTCBASE+0x00)
#define RTC_ISR         (RTCBASE+0x0C)
#define RTC_WPR         (RTCBASE+0x24)

#define GPIOBBASE       0x40020400
#define GPIOB_MODER     (GPIOBBASE+0x00)
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
    ra&=~(3<<( 0<<1)); //PB0
    ra|= (1<<( 0<<1)); //PB0
    ra&=~(3<<( 7<<1)); //PB7
    ra|= (1<<( 7<<1)); //PB7
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

static void rtc_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_APB1ENR);
    ra|=(1<<28); //PWR_EN
    PUT32(RCC_APB1ENR,ra);
    PUT32(PWR_CR,0x100);
    PUT32(RCC_BDCR,0x0019);
    while(1) if((GET32(RCC_BDCR)&2)) break;
    PUT32(RCC_BDCR,0x0119);
    PUT32(RCC_BDCR,0x8119);
    PUT32(RTC_WPR,0xCA);
    PUT32(RTC_WPR,0x53);

    PUT32(RTC_ISR,0x0087); //being lazy no rmw
    while(1)    if(GET32(RTC_ISR)&0x40) break;
    //keep RTC_PRER at 0x007F00FF for 32768Hz crystal
    //PUT32(RTC_TR,0x004123456); //set time and am/pm
    PUT32(RTC_ISR,0x0007); //being lazy no rmw
}

static void clock_init ( void )
{
    unsigned int ra;

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
}

static void do_delay ( unsigned int x )
{
    unsigned int ra;
    unsigned int rb;

    ra=GET32(RTC_TR);
    while(x--)
    {
        while(1)
        {
            rb=GET32(RTC_TR);
            if(rb!=ra) break;
        }
        ra=rb;
    }
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    led_init();
    clock_init();

    rtc_init();
    ra=GET32(RTC_TR);
    while(1)
    {
        rb=GET32(RTC_TR);
        if(rb!=ra) break;
    }
    while(1)
    {
        led_on();
        do_delay(5);
        led_off();
        do_delay(1);
    }
    return(0);
}


