
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000


//PA0 DIN
//PA1 CLK
//PA2 CE
//PA3 D/C

#define SET_MOSI    (1<<(0+0))
#define SET_CLK     (1<<(1+0))
#define SET_CS      (1<<(2+0))
#define SET_DC      (1<<(3+0))
#define RESET_MOSI  (1<<(0+16))
#define RESET_CLK   (1<<(1+16))
#define RESET_CS    (1<<(2+16))
#define RESET_DC    (1<<(3+16))

static void spi_delay ( void )
{
}

static void spi_dc ( unsigned int x )
{
    if(x) PUT32(GPIOABASE+0x18,SET_DC);
    else  PUT32(GPIOABASE+0x18,RESET_DC);
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

static void spi_command ( unsigned int cmd )
{
    unsigned int ra;
    unsigned int rb;

    spi_dc(0);
    spi_cs(0);
    spi_delay();
    for(rb=cmd,ra=0;ra<8;ra++,rb<<=1)
    {
        spi_mosi((rb>>7)&1);
        spi_delay();
        spi_clk(1);
        spi_delay();
        spi_clk(0);
        spi_delay();
    }
    spi_cs(1);
}

static void spi_data ( unsigned int data )
{
    unsigned int ra;
    unsigned int rb;

    spi_dc(1);
    spi_cs(0);
    spi_delay();
    for(rb=data,ra=0;ra<8;ra++,rb<<=1)
    {
        spi_mosi((rb>>7)&1);
        spi_delay();
        spi_clk(1);
        spi_delay();
        spi_clk(0);
        spi_delay();
    }
    spi_cs(1);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<0); //PA0
    ra&=~(3<<2); //PA1
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    ra&=~(3<<8); //PA4
    ra&=~(3<<10); //PA5
    ra|=1<<0; //PA0
    ra|=1<<2; //PA1
    ra|=1<<4; //PA2
    ra|=1<<6; //PA3
    ra|=1<<8; //PA4
    ra|=1<<10; //PA5
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    ra&=~(1<<4); //PA4
    ra&=~(1<<5); //PA5
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<0; //PA0
    ra|=3<<2; //PA1
    ra|=3<<4; //PA2
    ra|=3<<6; //PA3
    ra|=3<<8; //PA4
    ra|=3<<10; //PA5
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<0); //PA0
    ra&=~(3<<2); //PA1
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    ra&=~(3<<8); //PA4
    ra&=~(3<<10); //PA5
    PUT32(GPIOABASE+0x0C,ra);

    spi_clk(0);
    spi_data(0);
    spi_cs(0);
    spi_cs(1);

    
    spi_command(0x21); //extended commands
    spi_command(0xB0); //vop less contrast
//    spi_command(0xBF); //vop more contrast
    spi_command(0x04); //temp coef
    spi_command(0x14); //bias mode 1:48
    spi_command(0x20); //extended off
    spi_command(0x0C); //display on

//experiment 1
if(1)
{
    spi_command(0x80); //column
    spi_command(0x40); //row
    for(ra=0;ra<504;ra++) spi_data(ra);
}

    
    return(0);
}
