
//------------------------------------------------------------------------
//------------------------------------------------------------------------

#define BRICK_BUTTON

void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40021000
#define RCC_CR          (RCCBASE+0x00)
#define RCC_CFGR        (RCCBASE+0x04)
#define RCC_APB2RSTR    (RCCBASE+0x0C)
#define RCC_APB1RSTR    (RCCBASE+0x10)
#define RCC_AHBENR      (RCCBASE+0x14)
#define RCC_APB2ENR     (RCCBASE+0x18)
#define RCC_APB1ENR     (RCCBASE+0x1C)
#define RCC_CFGR2       (RCCBASE+0x2C)

#define GPIOABASE 0x48000000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_OTYPER    (GPIOABASE+0x04)
#define GPIOA_OSPEEDR   (GPIOABASE+0x08)
#define GPIOA_PUPDR     (GPIOABASE+0x0C)
#define GPIOA_IDR       (GPIOABASE+0x10)
#define GPIOA_ODR       (GPIOABASE+0x14)
#define GPIOA_BSRR      (GPIOABASE+0x18)
#define GPIOA_LCKR      (GPIOABASE+0x1C)
#define GPIOA_AFRL      (GPIOABASE+0x20)
#define GPIOA_AFRH      (GPIOABASE+0x24)
#define GPIOA_BRR       (GPIOABASE+0x28)

#define GPIOCBASE 0x48000800
#define GPIOC_MODER     (GPIOCBASE+0x00)
#define GPIOC_OTYPER    (GPIOCBASE+0x04)
#define GPIOC_OSPEEDR   (GPIOCBASE+0x08)
#define GPIOC_PUPDR     (GPIOCBASE+0x0C)
#define GPIOC_IDR       (GPIOCBASE+0x10)
#define GPIOC_ODR       (GPIOCBASE+0x14)
#define GPIOC_BSRR      (GPIOCBASE+0x18)
#define GPIOC_LCKR      (GPIOCBASE+0x1C)
#define GPIOC_AFRL      (GPIOCBASE+0x20)
#define GPIOC_AFRH      (GPIOCBASE+0x24)
#define GPIOC_BRR       (GPIOCBASE+0x28)

#define USART1BASE 0x40013800
#define USART1_CR1      (USART1BASE+0x00)
#define USART1_BRR      (USART1BASE+0x0C)
#define USART1_ISR      (USART1BASE+0x1C)
#define USART1_RDR      (USART1BASE+0x24)
#define USART1_TDR      (USART1BASE+0x28)

#define USART2BASE 0x40004400
#define USART2_CR1      (USART2BASE+0x00)
#define USART2_BRR      (USART2BASE+0x0C)
#define USART2_ISR      (USART2BASE+0x1C)
#define USART2_RDR      (USART2BASE+0x24)
#define USART2_TDR      (USART2BASE+0x28)

#define FLASHBASE 0x40022000
#define FLASH_ACR (FLASHBASE+0x00)

//PA9 is USART1_TX alternate function 1
//PA10 is USART1_RX alternate function 1

//PA2 is USART2_TX alternate function 1
//PA3 is USART2_RX alternate function 1

//------------------------------------------------------------------------
#ifdef BRICK_BUTTON
//------------------------------------------------------------------------
static void button_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHBENR);
    ra|=1<<19; //enable port C
    PUT32(RCC_AHBENR,ra);

    ra=GET32(GPIOC_MODER);
    ra&=~(3<<26); //PC13
    PUT32(GPIOC_MODER,ra);

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

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port A
    PUT32(RCC_AHBENR,ra);

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
    while(1) if((GET32(RCC_CFGR)&3)==1) break;





    //make sure hse is up, external 8mhz clock
    ra=GET32(RCC_CR);
    ra|=1<<16;
    PUT32(RCC_CR,ra);
    while(1) if(GET32(RCC_CR)&(1<<17)) break;
    //use the hse clock for now
    ra=GET32(RCC_CFGR);
    ra&=~3;
    ra|=1;
    PUT32(RCC_CFGR,ra);
    while(1) if((GET32(RCC_CFGR)&3)==1) break;


    //in order to change any pll settings must disable pll first
    ra=GET32(RCC_CR);
    ra&=~(1<<24);
    PUT32(RCC_CR,ra);
    while(1) if((GET32(RCC_CR)&(1<<25))==0) break;
    //no hse pre-division
    PUT32(RCC_CFGR2,0);
    //set other pll settings 6*8=48mhz, max speed for this part
    ra=GET32(RCC_CFGR);
    ra&=~(0x3F<<16);
    ra|=0x4<<18; //multiply by 6
    ra|=1<<16; //hse source
    PUT32(RCC_CFGR,ra);
    //turn pll back on with new settings and wait for it to be ready
    ra=GET32(RCC_CR);
    ra|=(1<<24);
    PUT32(RCC_CR,ra);
    while(1) if((GET32(RCC_CR)&(1<<25))!=0) break;
    //We cannot overclock the flash, so slow down the flash first
    //then speed up the clock
    //PUT32(FLASH_ACR,0x01); //>24Mhz no prefetch
    PUT32(FLASH_ACR,0x11); // >24MHZ with prefetch.
    //switch system to use pll clock
    ra=GET32(RCC_CFGR);
    ra&=~3;
    ra|=2;
    PUT32(RCC_CFGR,ra);
    while(1) if((GET32(RCC_CFGR)&3)==2) break;
    //now running at 48MHz.

    return(0);
}
//------------------------------------------------------------------------
int uart1_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port A
    PUT32(RCC_AHBENR,ra);

    ra=GET32(RCC_APB2ENR);
    ra|=1<<14; //enable USART1
    PUT32(RCC_APB2ENR,ra);

    //moder 10
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    ra|=2<<18; //PA9
    ra|=2<<20; //PA10
    PUT32(GPIOA_MODER,ra);
    //OTYPER 0
    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<9); //PA9
    ra&=~(1<<10); //PA10
    PUT32(GPIOA_OTYPER,ra);
    //ospeedr 11
    ra=GET32(GPIOA_OSPEEDR);
    ra|=3<<18; //PA9
    ra|=3<<20; //PA10
    PUT32(GPIOA_OSPEEDR,ra);
    //pupdr 00
    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    PUT32(GPIOA_PUPDR,ra);
    //afr 0001
    ra=GET32(GPIOA_AFRH);
    ra&=~(0xF<<4); //PA9
    ra&=~(0xF<<8); //PA10
    ra|=0x1<<4; //PA9
    ra|=0x1<<8; //PA10
    PUT32(GPIOA_AFRH,ra);




    ra=GET32(RCC_APB2RSTR);
    ra|=1<<14; //reset USART1
    PUT32(RCC_APB2RSTR,ra);
    ra&=~(1<<14);
    PUT32(RCC_APB2RSTR,ra);

    PUT32(USART1_BRR,417);
    PUT32(USART1_CR1,(1<<3)|(1<<2)|1);

    return(0);
}
//------------------------------------------------------------------------
void uart1_send ( unsigned int x )
{
    while(1) if(GET32(USART1_ISR)&(1<<7)) break;
    PUT32(USART1_TDR,x);
}
//------------------------------------------------------------------------
unsigned int uart1_recv ( void )
{
    while(1) if((GET32(USART1_ISR))&(1<<5)) break;
    return(GET32(USART1_RDR));
}
//------------------------------------------------------------------------
unsigned int uart1_check ( void )
{
    if((GET32(USART1_ISR))&(1<<5)) return(1);
    return(0);
}
//------------------------------------------------------------------------
int uart2_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port A
    PUT32(RCC_AHBENR,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<17; //enable USART2
    PUT32(RCC_APB1ENR,ra);

    //moder 10
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    ra|=2<<4; //PA2
    ra|=2<<6; //PA3
    PUT32(GPIOA_MODER,ra);
    //OTYPER 0
    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    PUT32(GPIOA_OTYPER,ra);
    //ospeedr 11
    ra=GET32(GPIOA_OSPEEDR);
    ra|=3<<4; //PA2
    ra|=3<<6; //PA3
    PUT32(GPIOA_OSPEEDR,ra);
    //pupdr 00
    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    PUT32(GPIOA_PUPDR,ra);
    //afr 0001
    ra=GET32(GPIOA_AFRL);
    ra&=~(0xF<<8); //PA2
    ra&=~(0xF<<12); //PA3
    ra|=0x1<<8; //PA2
    ra|=0x1<<12; //PA3
    PUT32(GPIOA_AFRL,ra);

    ra=GET32(RCC_APB1RSTR);
    ra|=1<<17; //reset USART2
    PUT32(RCC_APB1RSTR,ra);
    ra&=~(1<<17);
    PUT32(RCC_APB1RSTR,ra);

    PUT32(USART2_BRR,417);
    PUT32(USART2_CR1,(1<<3)|(1<<2)|1);

    return(0);
}
//------------------------------------------------------------------------
void uart2_send ( unsigned int x )
{
    while(1) if(GET32(USART2_ISR)&(1<<7)) break;
    PUT32(USART2_TDR,x);
}
//------------------------------------------------------------------------
unsigned int uart2_recv ( void )
{
    while(1) if((GET32(USART2_ISR))&(1<<5)) break;
    return(GET32(USART2_RDR));
}
//------------------------------------------------------------------------
unsigned int uart2_check ( void )
{
    if((GET32(USART2_ISR))&(1<<5)) return(1);
    return(0);
}
//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart2_send(rc);
        if(rb==0) break;
    }
    uart2_send(0x20);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart2_send(0x0D);
    uart2_send(0x0A);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    clock_init();
    uart1_init();
    uart2_init();

    hexstrings(0x1234); hexstring(0xABCD);
    hexstring(GET32(0x1FFFF7CC));
    hexstring(GET32(0x40015800));

    while(1)
    {
        if(uart1_check())
        {
            ra=uart1_recv();
            uart2_send(ra);
            if(ra==0x0D) uart2_send(0x0A);
        }
        if(uart2_check())
        {
            ra=uart2_recv();
            uart1_send(ra);
            if(ra==0x0D) uart1_send(0x0A);
        }
    }
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
