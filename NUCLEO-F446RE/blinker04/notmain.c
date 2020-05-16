
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define RCCBASE         0x40023800
#define RCC_AHB1ENR     (RCCBASE+0x30)

#define GPIOABASE       0x40020000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_BSRR      (GPIOABASE+0x18)

#define GPIOCBASE       0x40020800
#define GPIOC_MODER     (GPIOCBASE+0x00)
#define GPIOC_PUPDR     (GPIOCBASE+0x0C)
#define GPIOC_IDR       (GPIOCBASE+0x10)

//PA5
//PC13

static void led_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<0; //enable GPIOA
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(5<<1)); //PA5
    ra|= (1<<(5<<1)); //PA5
    PUT32(GPIOA_MODER,ra);
}

static void led_on ( void )
{
    PUT32(GPIOA_BSRR,((1<<5)<< 0));
}

static void led_off ( void )
{
    PUT32(GPIOA_BSRR,((1<<5)<<16));
}

static void button_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<2; //enable GPIOC
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(GPIOC_MODER);
    ra&=~(3<<26); //PC13
    PUT32(GPIOC_MODER,ra);

    ra=GET32(GPIOC_PUPDR);
    ra&=~(3<<26); //PC13
    PUT32(GPIOC_PUPDR,ra);
}

static unsigned int read_button ( void )
{
    return(GET32(GPIOC_IDR)&(1<<13));
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
