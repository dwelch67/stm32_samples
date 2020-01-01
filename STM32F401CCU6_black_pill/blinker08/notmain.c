
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void WFE ( void );

#define RCCBASE 0x40023800
#define RCC_AHB1ENR     (RCCBASE+0x30)

#define GPIOCBASE 0x40020800
#define GPIOC_MODER     (GPIOCBASE+0x00)
#define GPIOC_OTYPER    (GPIOCBASE+0x04)
#define GPIOC_BSRR      (GPIOCBASE+0x18)

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018

#define ICSR 0xE000ED04
#define SCR 0xE000ED10

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

static void led_on ( void )
{
    PUT32(GPIOC_BSRR,((1<<13)<<16));
}

static void led_off ( void )
{
    PUT32(GPIOC_BSRR,((1<<13)<< 0));
}

void systick_handler ( void )
{
    while(1) continue;
}

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(SCR);
    ra|=0x10;
    PUT32(SCR,ra);

    led_init();
    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0x00800000);
    PUT32(STK_CVR,0x00800000);
    PUT32(STK_CSR,0x00000007);

    while(1)
    {
        led_on();
        WFE();
        PUT32(ICSR,1<<25);
        led_off();
        WFE();
        PUT32(ICSR,1<<25);
    }

    return(0);
}


