
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

//#define GPIOABASE 0x50000000
//#define GPIOBBASE 0x50000400

//#define RCCBASE 0x40021000

//#define FLASHBASE 0x40022000


//#define GPIOA_BSRR (GPIOABASE+0x18)

#define GPIOCBASE 0x40011000
#define RCCBASE 0x40021000


#define GPIOC_BSRR (GPIOCBASE+0x10)


#define CS      13
#define MOSI    14
#define SCK     15

#define SPI_CS_ON    ( PUT32(GPIOC_BSRR,1<<(CS  + 0)) )
#define SPI_CS_OFF   ( PUT32(GPIOC_BSRR,1<<(CS  +16)) )

#define SPI_CLK_ON   ( PUT32(GPIOC_BSRR,1<<(SCK + 0)) )
#define SPI_CLK_OFF  ( PUT32(GPIOC_BSRR,1<<(SCK +16)) )

#define SPI_MOSI_ON  ( PUT32(GPIOC_BSRR,1<<(MOSI+ 0)) )
#define SPI_MOSI_OFF ( PUT32(GPIOC_BSRR,1<<(MOSI+16)) )


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



    ra=GET32(RCCBASE+0x18);
    ra|=1<<4; //enable port c
    PUT32(RCCBASE+0x18,ra);
    //config
    ra=GET32(GPIOCBASE+0x04);
    ra&=~(3<<20);   //PC13
    ra|=  1<<20;    //PC13
    ra&=~(3<<22);   //PC13
    ra|=  0<<22;    //PC13
    ra&=~(3<<24);   //PC13
    ra|=  1<<24;    //PC13
    ra&=~(3<<26);   //PC13
    ra|=  0<<26;    //PC13
    ra&=~(3<<28);   //PC13
    ra|=  1<<28;    //PC13
    ra&=~(3<<30);   //PC13
    ra|=  0<<30;    //PC13
    PUT32(GPIOCBASE+0x04,ra);



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

    return(0);
}

/*

halt
flash write_image erase sharp01/notmain.elf
reset

*/


