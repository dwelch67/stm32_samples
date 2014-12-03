
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOCBASE 0x48000800
#define RCCBASE 0x40021000
#define RCC_APB1ENR (RCCBASE+0x1C)

#define TIM6BASE  0x40001000
#define TIM6_CR1  (TIM6BASE+0x00)
#define TIM6_CR2  (TIM6BASE+0x04)
#define TIM6_DIER (TIM6BASE+0x0C)
#define TIM6_SR   (TIM6BASE+0x10)
#define TIM6_EGR  (TIM6BASE+0x14)
#define TIM6_CNT  (TIM6BASE+0x24)
#define TIM6_PSC  (TIM6BASE+0x28)
#define TIM6_ARR  (TIM6BASE+0x2C)

static void led_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port A
    PUT32(RCCBASE+0x14,ra);

    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<10); //PA5
    ra|=1<<10; //PA5
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<5); //PA5
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<10; //PA5
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<10); //PA5
    PUT32(GPIOABASE+0x0C,ra);
}

static void led_on ( void )
{
    PUT32(GPIOABASE+0x18,((1<<5)<<0));
}

static void led_off ( void )
{
    PUT32(GPIOABASE+0x18,((1<<5)<<16));
}

static void button_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<19; //enable port C
    PUT32(RCCBASE+0x14,ra);

    //moder 2b00
    ra=GET32(GPIOCBASE+0x00);
    ra&=~(3<<26); //PC13
    PUT32(GPIOCBASE+0x00,ra);

    //pupdr 2b00
    ra=GET32(GPIOCBASE+0x0C);
    ra&=~(3<<26); //PC13
    PUT32(GPIOCBASE+0x0C,ra);
}

static unsigned int read_button ( void )
{
    return(GET32(GPIOCBASE+0x10)&(1<<13));
}

static void timer_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x1C);
    ra|=1<<4; //tim6 enable
    PUT32(RCCBASE+0x1C,ra);

    PUT16(TIM6_CR1,0x0000);
    PUT16(TIM6_DIER,0x0000);
    PUT16(TIM6_PSC,0xFFFF);
    PUT16(TIM6_ARR,122);
    PUT16(TIM6_CNT,122);
    PUT16(TIM6_CR1,0x0001);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    led_init();
    button_init();

    //this is a safety measure, if the code below that messes with
    //the clock source, bricks the microcontroller then press and
    //hold the user button, and press reset, on that reset this code
    //will see the button pressed and go into this infinite loop
    //instead of into the code below, allowing us to reprogram the
    //microcontrollers flash with hopefully a not buggy program.
    if(read_button()==0)
    {
        //make the led glow
        while(1)
        {
            led_on();
            led_off();
        }
    }
    //as delivered this code does not brick the microcontroller
    //the above was for me to develop this code and for you to change
    //this code as desired.

    //switch to external clock.
    ra=GET32(RCCBASE+0x00);
    ra|=1<<16;
    PUT32(RCCBASE+0x00,ra);
    while(1) if(GET32(RCCBASE+0x00)&(1<<17)) break;
    ra=GET32(RCCBASE+0x04);
    ra&=~3;
    ra|=1;
    PUT32(RCCBASE+0x04,ra);
    while(1) if((GET32(RCCBASE+0x04)&3)==1) break;

    timer_init();

    while(1)
    {
        led_on();
        for(rx=0;rx<10;rx++)
        {
            while(1) if(GET16(TIM6_SR)&1) break;
            PUT16(TIM6_SR,0);
        }
        led_off();
        while(1) if(GET16(TIM6_SR)&1) break;
        PUT16(TIM6_SR,0);
    }
    return(0);
}
