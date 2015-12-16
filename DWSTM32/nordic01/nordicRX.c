
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#define USART1BASE 0x40013800

//PA2 IRQ
//PA3 CE

//PA4 NSS
//PA5 SCK
//PA6 MISO
//PA7 MOSI

#define SET_CE      (1<<(3+0))
#define RESET_CE    (1<<(3+16))

#define SET_CS      (1<<(4+0))
#define SET_CLK     (1<<(5+0))
#define SET_MOSI    (1<<(7+0))

#define RESET_CS    (1<<(4+16))
#define RESET_CLK   (1<<(5+16))
#define RESET_MOSI  (1<<(7+16))

#define MISO_BIT    (1<<6)
#define IRQ_BIT     (1<<2)

static void nordic_gpio_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    //ra|=1<<4; //PA3
    ra|=1<<6; //PA3
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<3); //PA3
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<6; //PA3
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<4); //PA2
    ra|=1<<4; //PA2
    ra&=~(3<<6); //PA3
    PUT32(GPIOABASE+0x0C,ra);
}
static void nordic_ce ( unsigned int x )
{
    if(x) PUT32(GPIOABASE+0x18,SET_CE);
    else  PUT32(GPIOABASE+0x18,RESET_CE);
}
static unsigned int nordic_irq ( void )
{
    return(GET32(GPIOABASE+0x10)&IRQ_BIT);
}
static void spi_delay ( void )
{
}
static void spi_gpio_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<8); //PA4
    ra&=~(3<<10); //PA5
    ra&=~(3<<12); //PA6
    ra&=~(3<<14); //PA7
    ra|=1<<8; //PA4
    ra|=1<<10; //PA5
    //ra|=1<<12; //PA6
    ra|=1<<14; //PA7
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<4); //PA4
    ra&=~(1<<5); //PA5
    //ra&=~(1<<6); //PA6
    ra&=~(1<<7); //PA7
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<8; //PA4
    ra|=3<<10; //PA5
    //ra|=3<<12; //PA6
    ra|=3<<14; //PA7
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<8); //PA4
    ra&=~(3<<10); //PA5
    ra&=~(3<<12); //PA6
    ra|=1<<12; //PA6
    ra&=~(3<<14); //PA7
    PUT32(GPIOABASE+0x0C,ra);
}
static void spi_cs ( unsigned int x )
{
    if(x) PUT32(GPIOABASE+0x18,SET_CS);
    else  PUT32(GPIOABASE+0x18,RESET_CS);
}
static void spi_clk ( unsigned int x )
{
    if(x) PUT32(GPIOABASE+0x18,SET_CLK);
    else  PUT32(GPIOABASE+0x18,RESET_CLK);
}
static void spi_mosi ( unsigned int x )
{
    if(x) PUT32(GPIOABASE+0x18,SET_MOSI);
    else  PUT32(GPIOABASE+0x18,RESET_MOSI);
}
static unsigned int spi_miso ( void )
{
    return(GET32(GPIOABASE+0x10)&MISO_BIT);
}

static unsigned int spi8 ( unsigned int data  )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=data;
    rc=0;
    spi_cs(0);
    spi_delay();
    for(ra=0;ra<8;ra++)
    {
        spi_mosi((rb>>7)&1);
        rb<<=1;
        spi_delay();
        spi_clk(1);
        spi_delay();
        rc<<=1;
        if(spi_miso()) rc|=1;
        spi_clk(0);
        spi_delay();
    }
    spi_cs(1);
    return(rc);
}

static unsigned int spi16 ( unsigned int data )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=data;
    rc=0;
    spi_cs(0);
    spi_delay();
    for(ra=0;ra<16;ra++)
    {
        spi_mosi((rb>>15)&1);
        rb<<=1;
        spi_delay();
        spi_clk(1);
        spi_delay();
        rc<<=1;
        if(spi_miso()) rc|=1;
        spi_clk(0);
        spi_delay();
    }
    spi_cs(1);
    return(rc);
}

static unsigned int spi_transfer_four ( unsigned int command, unsigned int data )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=data;
    rc=0;
    spi_cs(0);
    spi_delay();
    rb=command;
    for(ra=0;ra<8;ra++)
    {
        spi_mosi((rb>>7)&1);
        rb<<=1;
        spi_delay();
        spi_clk(1);
        spi_delay();
        rc<<=1;
        if(spi_miso()) rc|=1;
        spi_clk(0);
        spi_delay();
    }
    rb=data;
    for(ra=0;ra<32;ra++)
    {
        spi_mosi((rb>>31)&1);
        rb<<=1;
        spi_delay();
        spi_clk(1);
        spi_delay();
        rc<<=1;
        if(spi_miso()) rc|=1;
        spi_clk(0);
        spi_delay();
    }
    spi_cs(1);
    return(rc);
}

//------------------------------------------------------------------------
int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x18,ra);

    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<18); //PA9
    ra|=2<<18; //PA9
    ra&=~(3<<20); //PA10
    ra|=2<<20; //PA10
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER 0
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<9); //PA9
    ra&=~(1<<10); //PA10
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr 11
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<18; //PA9
    ra|=3<<20; //PA10
    PUT32(GPIOABASE+0x08,ra);
    //pupdr 00
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    PUT32(GPIOABASE+0x0C,ra);
    //afr 0001
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x1<<4; //PA9
    ra&=~(0xF<<8); //PA10
    ra|=0x1<<8; //PA10
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x0C);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x0C,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x0C,ra);

    PUT32(USART1BASE+0x0C,69);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
}
//------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    return(GET32(USART1BASE+0x24));
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
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
int notmain ( void )
{
    unsigned int ra;

    uart_init();
    hexstring(0x12345678);
    spi_gpio_init();
    nordic_gpio_init();

    spi_clk(0);
    spi_mosi(0);
    spi_cs(1);

    hexstring(spi16(0));

    nordic_ce(0); //standby mode

    spi16(0x2039); //crc 1 byte, power down, prx
    spi16(0x2100); //Disable auto-acknowledge
    spi16(0x2770); //clear what you can
    spi16(0x3104); //4 byte receive payload
    spi16(0x203B); //power up

    nordic_ce(1); //start rx

    hexstring(spi16(0));

    while(1)
    {
        if(1)
        {
            if(nordic_irq()!=0) continue;
            ra=spi8(0xFF);
        }
        else
        {
            ra=spi8(0xFF);
            if((ra&0x40)==0) continue;
        }
        hexstring(ra);

        nordic_ce(0); //standby
        ra=spi_transfer_four(0x61,0xFFFFFFFF); //Set RX pipe 0 address
        hexstring(ra);
        spi8(0xE2); //flush rx fifo
        spi16(0x2740); //clear interrupt
        nordic_ce(1); //start rx
    }


    return(0);
}
