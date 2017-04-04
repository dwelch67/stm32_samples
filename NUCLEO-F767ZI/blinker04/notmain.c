
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );



#define RCCBASE 0x40023800
#define RCC_AHB1ENR (RCCBASE+0x30)
#define RCC_APB1ENR (RCCBASE+0x40)

#define GPIOABASE 0x40020000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_OTYPER    (GPIOABASE+0x04)
#define GPIOA_OSPEEDR   (GPIOABASE+0x08)
#define GPIOA_PUPDR     (GPIOABASE+0x0C)
#define GPIOA_BSRR      (GPIOABASE+0x18)

#define GPIOCBASE 0x40020800
#define GPIOC_MODER     (GPIOCBASE+0x00)
#define GPIOC_OTYPER    (GPIOCBASE+0x04)
#define GPIOC_OSPEEDR   (GPIOCBASE+0x08)
#define GPIOC_PUPDR     (GPIOCBASE+0x0C)
#define GPIOC_IDR       (GPIOCBASE+0x10)

#define GPIOBBASE 0x40020400
#define GPIOB_MODER     (GPIOBBASE+0x00)
#define GPIOB_OTYPER    (GPIOBBASE+0x04)
#define GPIOB_BSRR      (GPIOBBASE+0x18)

//PA5 or PB0 defaults to PB0
//PB7
//PB14


static void led_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<1; //enable GPIOB
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(GPIOB_MODER);
    ra&=~(3<<(0<<1)); //PB0
    ra|= (1<<(0<<1)); //PB0
    ra&=~(3<<(7<<1)); //PB7
    ra|= (1<<(7<<1)); //PB7
    ra&=~(3<<(14<<1)); //PB14
    ra|= (1<<(14<<1)); //PB14
    PUT32(GPIOB_MODER,ra);
    //OTYPER
    ra=GET32(GPIOB_OTYPER);
    ra&=~(1<<0); //PB0
    ra&=~(1<<7); //PB7
    ra&=~(1<<14); //PB14
    PUT32(GPIOB_OTYPER,ra);
}

static void led_on ( void )
{
    PUT32(GPIOB_BSRR,((1<<0)<<0)|((1<<7)<<0)|((1<<14)<<0));
}

static void led_off ( void )
{
    PUT32(GPIOB_BSRR,((1<<0)<<16)|((1<<7)<<16)|((1<<14)<<16));
}


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


