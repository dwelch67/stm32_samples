
//------------------------------------------------------------------------
//------------------------------------------------------------------------

#define BRICK_BUTTON

void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40023800
#define RCC_CR          (RCCBASE+0x00)
#define RCC_CFGR        (RCCBASE+0x08)
#define RCC_APB1RSTR    (RCCBASE+0x20)
#define RCC_AHB1ENR     (RCCBASE+0x30)
#define RCC_APB1ENR     (RCCBASE+0x40)
#define RCC_BDCR        (RCCBASE+0x70)

#define GPIOABASE 0x40020000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_OTYPER    (GPIOABASE+0x04)
#define GPIOA_OSPEEDR   (GPIOABASE+0x08)
#define GPIOA_PUPDR     (GPIOABASE+0x0C)
#define GPIOA_BSRR      (GPIOABASE+0x18)
#define GPIOA_AFRL      (GPIOABASE+0x20)

#define GPIOCBASE 0x40020800
#define GPIOC_MODER     (GPIOCBASE+0x00)
#define GPIOC_OTYPER    (GPIOCBASE+0x04)
#define GPIOC_OSPEEDR   (GPIOCBASE+0x08)
#define GPIOC_PUPDR     (GPIOCBASE+0x0C)
#define GPIOC_IDR       (GPIOCBASE+0x10)

#define USART2BASE 0x40004400
#define USART2_SR       (USART2BASE+0x00)
#define USART2_DR       (USART2BASE+0x04)
#define USART2_BRR      (USART2BASE+0x08)
#define USART2_CR1      (USART2BASE+0x0C)

//PA2 is USART2_TX alternate function 1
//PA3 is USART2_RX alternate function 1

//------------------------------------------------------------------------
#ifdef BRICK_BUTTON
//------------------------------------------------------------------------
static void button_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<2; //enable GPIOC
    PUT32(RCC_AHB1ENR,ra);

    //moder 2b00
    ra=GET32(GPIOC_MODER);
    ra&=~(3<<26); //PC13
    PUT32(GPIOC_MODER,ra);

    //pupdr 2b00
    ra=GET32(GPIOC_PUPDR);
    ra&=~(3<<26); //PC13
    PUT32(GPIOC_PUPDR,ra);
}
//------------------------------------------------------------------------
static unsigned int read_button ( void )
{
    return(GET32(GPIOC_IDR)&(1<<13));
}
//------------------------------------------------------------------------
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

    ra=GET32(GPIOA_OSPEEDR);
    ra|=3<<10; //PA5
    PUT32(GPIOA_OSPEEDR,ra);
    //pupdr
    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<<10); //PA5
    PUT32(GPIOA_PUPDR,ra);
}
//------------------------------------------------------------------------
static void led_on ( void )
{
    PUT32(GPIOA_BSRR,((1<<5)<<0));
}
//------------------------------------------------------------------------
static void led_off ( void )
{
    PUT32(GPIOA_BSRR,((1<<5)<<16));
}
//------------------------------------------------------------------------
#endif //BRICK_BUTTON
//------------------------------------------------------------------------
static int clock_init ( void )
{
    unsigned int ra;

#ifdef BRICK_BUTTON
    //this is a safety measure, if the code below that messes with
    //the clock source, bricks the microcontroller then press and
    //hold the user button, and press reset, on that reset this code
    //will see the button pressed and go into this infinite loop
    //instead of into the code below, allowing us to reprogram the
    //microcontrollers flash with hopefully a not buggy program.
    button_init();
    if(read_button()==0)
    {
        //make the led glow
        led_init();
        while(1)
        {
            led_on();
            led_off();
        }
    }
    //as delivered this code does not brick the microcontroller
    //the above was for me to develop this code and for you to change
    //this code as desired.
#endif //BRICK_BUTTON

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

    return(0);
}
//------------------------------------------------------------------------
int uart2_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<0; //enable port A
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<17; //enable USART2
    PUT32(RCC_APB1ENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    ra|=2<<4; //PA2
    ra|=2<<6; //PA3
    PUT32(GPIOA_MODER,ra);

    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    PUT32(GPIOA_OTYPER,ra);

    ra=GET32(GPIOA_OSPEEDR);
    ra|=3<<4; //PA2
    ra|=3<<6; //PA3
    PUT32(GPIOA_OSPEEDR,ra);

    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    PUT32(GPIOA_PUPDR,ra);

    ra=GET32(GPIOA_AFRL);
    ra&=~(0xF<<8); //PA2
    ra&=~(0xF<<12); //PA3
    ra|=0x7<<8; //PA2
    ra|=0x7<<12; //PA3
    PUT32(GPIOA_AFRL,ra);

    ra=GET32(RCC_APB1RSTR);
    ra|=1<<17; //reset USART2
    PUT32(RCC_APB1RSTR,ra);
    ra&=~(1<<17);
    PUT32(RCC_APB1RSTR,ra);

    //PUT32(USART2_CR1,(1<<13));

    //8000000/(16*115200) = 4.34  4+5/16
    PUT32(USART2_BRR,0x45);
    PUT32(USART2_CR1,(1<<3)|(1<<2)|(1<<13));

    return(0);
}
//------------------------------------------------------------------------
void uart2_send ( unsigned int x )
{
    while(1) if(GET32(USART2_SR)&(1<<7)) break;
    PUT32(USART2_DR,x);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int rx;

    clock_init();
    uart2_init();
    for(rx=0;;rx++)
    {
        uart2_send(0x30+(rx&7));
    }
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
