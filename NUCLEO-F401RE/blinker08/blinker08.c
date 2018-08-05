
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void WFE ( void );

#define RCCBASE 0x40023800
//#define RCC_CR          (RCCBASE+0x00)
//#define RCC_CFGR        (RCCBASE+0x08)
#define RCC_AHB1ENR     (RCCBASE+0x30)
//#define RCC_APB1ENR     (RCCBASE+0x40)
//#define RCC_BDCR        (RCCBASE+0x70)

#define GPIOABASE 0x40020000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_OTYPER    (GPIOABASE+0x04)
//#define GPIOA_OSPEEDR   (GPIOABASE+0x08)
//#define GPIOA_PUPDR     (GPIOABASE+0x0C)
#define GPIOA_BSRR      (GPIOABASE+0x18)

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018

#define ICSR 0xE000ED04
#define SCR 0xE000ED10

static void led_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<0; //enable GPIOA
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<10); //PA5
    ra|=1<<10; //PA5
    PUT32(GPIOA_MODER,ra);

    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<5); //PA5
    PUT32(GPIOA_OTYPER,ra);
}

static void led_on ( void )
{
    PUT32(GPIOA_BSRR,((1<<5)<<0));
}

static void led_off ( void )
{
    PUT32(GPIOA_BSRR,((1<<5)<<16));
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


