
//------------------------------------------------------------------------
//------------------------------------------------------------------------

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40023800
#define RCC_AHB1ENR (RCCBASE+0x30)

#define GPIOCBASE 0x40020800
#define GPIOC_MODER     (GPIOCBASE+0x00)
#define GPIOC_OTYPER    (GPIOCBASE+0x04)
#define GPIOC_OSPEEDR   (GPIOCBASE+0x08)
#define GPIOC_PUPDR     (GPIOCBASE+0x0C)
#define GPIOC_BSRR      (GPIOCBASE+0x18)

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018

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
//------------------------------------------------------------------------
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
//------------------------------------------------------------------------
//------------------------------------------------------------------------
