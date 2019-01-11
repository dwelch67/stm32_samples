
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );


#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400

#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define CLK 1
#define DIO 0

static void cd_delay ( void )
{
    unsigned int ra;
    for(ra=0;ra<100;ra++) dummy(ra);
}

static void clk_high ( void )
{
    PUT32(GPIOABASE+0x18,1<<(CLK+ 0));
}
static void clk_low ( void )
{
    PUT32(GPIOABASE+0x18,1<<(CLK+16));
}
static void dio_high ( void )
{
    PUT32(GPIOABASE+0x18,1<<(DIO+ 0));
}
static void dio_low ( void )
{
    PUT32(GPIOABASE+0x18,1<<(DIO+16));
}
static unsigned int dio_read ( void )
{
    return(GET32(GPIOABASE+0x10)&1<<DIO);
}
static void dio_input ( void )
{
    unsigned int ra;

    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(DIO<<1));
    ra|= (0<<(DIO<<1));
    PUT32(GPIOABASE+0x00,ra);
}
static void dio_output ( void )
{
    unsigned int ra;

    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(DIO<<1));
    ra|= (1<<(DIO<<1));
    PUT32(GPIOABASE+0x00,ra);
}

static void cd_start ( void )
{
    dio_low();
    cd_delay();
}

static void cd_stop ( void )
{
    clk_high();
    cd_delay();
    dio_high();
    cd_delay();
}

static unsigned int cd_byte ( unsigned int data )
{
    unsigned int rm;

    for(rm=0x01;rm<0x100;rm<<=1)
    {
        clk_low();
        cd_delay();
        if(data&rm) dio_high();
        else        dio_low();
        cd_delay();
        clk_high();
        cd_delay();
    }

    clk_low();
    cd_delay();
    dio_input();
    cd_delay();
    clk_high();
    cd_delay();
    rm=dio_read();
    clk_low();
    cd_delay();
    dio_output();
    return(rm);
}


int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(DIO<<1));
    ra|= (1<<(DIO<<1));
    ra&=~(3<<(CLK<<1));
    ra|= (1<<(CLK<<1));
    PUT32(GPIOABASE+0x00,ra);

    //PA0 DATA PA1 CLOCK
    //otyper
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<DIO);
    ra&=~(1<<CLK);
    PUT32(GPIOABASE+0x04,ra);

    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<(DIO<<1));
    ra|= (1<<(DIO<<1)); //pull up
    PUT32(GPIOABASE+0x0C,ra);

    clk_high();
    cd_delay();
    dio_high();
    cd_delay();
    dio_low();
    cd_delay();
    dio_high();
    cd_delay();
    //known state?

    cd_start();
    cd_byte(0x40);
    cd_stop();

    cd_start();
    cd_byte(0xC0);
    cd_byte(0x06);
    cd_byte(0x5B); //orr with 0x80 to set colon
    cd_byte(0x4F);
    cd_byte(0x66);
    cd_stop();

    cd_start();
    cd_byte(0x88);
    cd_stop();

    return(0);
}
