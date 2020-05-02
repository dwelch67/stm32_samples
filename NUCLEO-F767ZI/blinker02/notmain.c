
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define RCCBASE 0x40023800
#define RCC_AHB1ENR (RCCBASE+0x30)

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

//PB0
//PB7
//PB14

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018

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

void do_delay ( unsigned int sec )
{
    unsigned int ra,rb,rc,rd;

    rb=GET32(STK_CVR);
    for(rd=0;rd<sec;)
    {
        ra=GET32(STK_CVR);
        rc=(rb-ra)&0x00FFFFFF;
        if(rc>=16000000)
        {
            rb=ra;
            rd++;
        }
    }
}

int notmain ( void )
{
    unsigned int rx;

    led_init();

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CSR,0x00000005);

    for(rx=0;rx<5;rx++)
    {
        led_on();
        while(1) if(GET32(STK_CVR)&0x200000) break;
        led_off();
        while(1) if((GET32(STK_CVR)&0x200000)==0) break;
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


