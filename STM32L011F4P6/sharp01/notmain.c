
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void DOWFI ( void );

#define GPIOABASE 0x50000000
#define GPIOBBASE 0x50000400

#define RCCBASE 0x40021000

#define FLASHBASE 0x40022000


#define GPIOA_BSRR (GPIOABASE+0x18)



#define CS      0
#define MOSI    1
#define SCK     2

#define SPI_CS_ON    ( PUT32(GPIOA_BSRR,1<<(CS  + 0)) )
#define SPI_CS_OFF   ( PUT32(GPIOA_BSRR,1<<(CS  +16)) )

#define SPI_CLK_ON   ( PUT32(GPIOA_BSRR,1<<(SCK + 0)) )
#define SPI_CLK_OFF  ( PUT32(GPIOA_BSRR,1<<(SCK +16)) )

#define SPI_MOSI_ON  ( PUT32(GPIOA_BSRR,1<<(MOSI+ 0)) )
#define SPI_MOSI_OFF ( PUT32(GPIOA_BSRR,1<<(MOSI+16)) )


static void spi_delay ( void )
{
    //unsigned short ra;
    //for(ra=0;ra<10;ra++) dummy(ra);
}

static void spi_write_byte ( unsigned char d )
{
    unsigned char ca;

    for(ca=0x80;ca;ca>>=1)
    {
        if(ca&d) SPI_MOSI_ON; else SPI_MOSI_OFF;
        spi_delay();
        SPI_CLK_ON;
        spi_delay();
        SPI_CLK_OFF;
        spi_delay();
    }
}

static void spi_write_sharp_address ( unsigned char d )
{
    unsigned char ca;

    for(ca=0x01;;ca<<=1)
    {
        if(ca&d) SPI_MOSI_ON; else SPI_MOSI_OFF;
        spi_delay();
        SPI_CLK_ON;
        spi_delay();
        SPI_CLK_OFF;
        spi_delay();
        if(ca==0x80) break;
    }
}

static void clear_screen ( void )
{
    SPI_CS_ON;
    spi_delay();
    spi_write_byte(0x20);
    spi_write_byte(0x00);
    spi_delay();
    SPI_CS_OFF;
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    unsigned int d;

    //SLOOOOW...
    ra=GET32(RCCBASE+0x04);
    ra&=~(7<<13);
    PUT32(RCCBASE+0x04,ra);

    //shut down flash
    PUT32(FLASHBASE+0x08,0x04152637);
    PUT32(FLASHBASE+0x08,0xFAFBFCFD);
    PUT32(FLASHBASE+0x00,0x00000018);


       //GND TX RX VCC

//BOOT0                SWCLK
//PF0                  SWDIO
//PF1                  RX
//NRST                 TX
//VDDA                 VCC
//PA0  cs              GND
//PA1  mosi            PB1
//PA2  sclk            PA7
//PA3                  PA6
//PA4 (led)            PA5 (led)


    ra=GET32(RCCBASE+0x2C);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x2C,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<0); //PA0
    ra&=~(3<<2); //PA1
    ra&=~(3<<4); //PA2
    ra|=1<<0; //PA0
    ra|=1<<2; //PA1
    ra|=1<<4; //PA2
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    ra&=~(1<<2); //PA2
    PUT32(GPIOABASE+0x04,ra);


    SPI_CS_OFF  ;
    SPI_CLK_OFF ;
    SPI_MOSI_OFF;


    clear_screen();

    //168x144

//    for(d=0xF0;;d++)
    d=0xF0;
    {
        for(ra=1;ra<=168;ra++)
        {
            spi_delay();
            SPI_CS_ON;
            spi_delay();
            spi_write_byte(0x80);
            spi_write_sharp_address(ra&0xFF);
            for(rb=0;rb<18;rb++)
            {
                spi_write_byte(d);
            }
            spi_write_byte(0x00);
            spi_write_byte(0x00);
            spi_delay();
            SPI_CS_OFF;
            spi_delay();
        }
    }

    while(1)
    {
        DOWFI();
    }



    return(0);
}
