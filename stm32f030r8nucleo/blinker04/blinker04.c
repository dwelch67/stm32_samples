
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOCBASE 0x48000800
#define RCCBASE 0x40021000

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

int notmain ( void )
{
    led_init();
    button_init();

    while(1)
    {
        if(read_button())
        {
            led_off();
        }
        else
        {
            led_on();
        }
    }
    return(0);
}
