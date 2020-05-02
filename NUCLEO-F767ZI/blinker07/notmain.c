
//Only use interrupts as a last resort.

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define RCCBASE         0x40023800
#define RCC_CR          (RCCBASE+0x00)
#define RCC_CFGR        (RCCBASE+0x08)
#define RCC_AHB1ENR     (RCCBASE+0x30)

#define STK_CSR         0xE000E010
#define STK_RVR         0xE000E014
#define STK_CVR         0xE000E018

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

static int clock_init ( void )
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

    return(0);
}

//********** interrupt service routine *************
volatile unsigned int tick;
void systick_handler ( void )
{
    GET32(STK_CSR);
    if((tick++)&1)  led_on();
    else            led_off();
}
//********** interrupt service routine *************

int notmain ( void )
{
    led_init();
    clock_init();
    tick=0;
    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,8000000-1);
    PUT32(STK_CVR,8000000-1);
    PUT32(STK_CSR,0x00000007);

    return(0);
}
