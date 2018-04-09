
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

#define RCC_BASE 0x40021000
#define RCC_APB2ENR (RCC_BASE+0x18)
#define RCC_APB1ENR (RCC_BASE+0x1C)
#define RCC_CR      (RCC_BASE+0x00)
#define RCC_CFGR    (RCC_BASE+0x04)
#define FLASH_ACR   0x40022000


static void clock_init ( void )
{
    unsigned int ra;

    //enable the external clock
    ra=GET32(RCC_CR);
    ra=ra|1<<16; //HSEON
    PUT32(RCC_CR,ra);

    //wait for HSE to settle
    while(1) if(GET32(RCC_CR)&(1<<17)) break; //HSERDY
    if(0)
    {
        //select HSE clock
        ra=GET32(RCC_CFGR);
        ra&=~(0x3<<0);
        ra|= (0x1<<0);
        PUT32(RCC_CFGR,ra);
        //wait for it
        while(1) if((GET32(RCC_CFGR)&0xF)==0x5) break;
        return;
    }
    //setup pll source settings
    ra=GET32(RCC_CFGR);
    ra&=~(0xF<<18);
    ra|= (0x7<<18);
    ra&=~(1<<17);
    ra|= (0<<17);
    ra&=~(1<<16);
    ra|= (1<<16);
    PUT32(RCC_CFGR,ra);
    //enable the pll
    ra=GET32(RCC_CR);
    ra=ra|1<<24; //PLLON
    PUT32(RCC_CR,ra);
    //wait for pll to lock
    while(1) if(GET32(RCC_CR)&(1<<25)) break; //HSERDY
    //USB div/1.5 AHB not divided, APB not divided, APB1 div/2, MCO, do you need this?
    ra=GET32(RCC_CFGR);
    ra&=~(0x7<<24);
    ra|= (0x7<<24);
    ra&=~(0x1<<22);
    ra|= (0x0<<22);
    ra&=~(0x7<<11);
    ra|= (0x0<<11);
    ra&=~(0x7<<8);
    ra|= (0x4<<8);
    ra&=~(0xF<<4);
    ra|= (0x0<<4);
    PUT32(RCC_CFGR,ra);
    //add wait states for the flash
    PUT32(FLASH_ACR,0x2);
    //switch to the pll as the source
    ra=GET32(RCC_CFGR);
    ra&=~(0x3<<0);
    ra|= (0x2<<0);
    PUT32(RCC_CFGR,ra);
    //wait for it
    while(1) if((GET32(RCC_CFGR)&0xF)==0xA) break;
}


const unsigned char font[16][8]=
{
{
0xFE, //#### ###.
0x82, //#... ..#.
0x82, //#... ..#.
0x82, //#... ..#.
0x82, //#... ..#.
0x82, //#... ..#.
0xFE, //#### ###.
0x00, //.........
},
{
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x00, //.... ....
},
{
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0xFE, //#### ###.
0x80, //#... ....
0x80, //#... ....
0xFE, //#### ###.
0x00, //.........
},
{
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0xFE, //#### ###.
0x00, //.........
},
{
0x82, //#... ..#.
0x82, //#... ..#.
0x82, //#... ..#.
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x00, //.........
},
{
0xFE, //#### ###.
0x80, //#... ....
0x80, //#... ....
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0xFE, //#### ###.
0x00, //.........
},
{
0xFE, //#### ###.
0x80, //#... ....
0x80, //#... ....
0xFE, //#### ###.
0x82, //#... ..#.
0x82, //#... ..#.
0xFE, //#### ###.
0x00, //.........
},
{
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x00, //.........
},
{
0xFE, //#### ###.
0x82, //#... ..#.
0x82, //#... ..#.
0xFE, //#### ###.
0x82, //#... ..#.
0x82, //#... ..#.
0xFE, //#### ###.
0x00, //.........
},
{
0xFE, //#### ###.
0x82, //#... ..#.
0x82, //#... ..#.
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x00, //.........
},

{
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
},

{
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
},

{
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
},

{
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
},

{
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
},

{
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
},

};




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
    unsigned int rc;
    unsigned int rd;

    unsigned int d;

    clock_init();

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


    for(ra=0;ra<168;ra++)
    {
        spi_delay();
        SPI_CS_ON;
        spi_delay();
        spi_write_byte(0x80);
        spi_write_sharp_address((ra+1)&0xFF);
        for(rb=0;rb<18;rb++)
        {
            spi_write_byte(0);
        }
        spi_write_byte(0x00);
        spi_write_byte(0x00);
        spi_delay();
        SPI_CS_OFF;
        spi_delay();
    }


    d=0;
    while(1)
    {
        for(ra=0;ra<64;ra++)
        {
            spi_delay();
            SPI_CS_ON;
            spi_delay();
            spi_write_byte(0x80);
            spi_write_sharp_address((ra+10+1)&0xFF);
            spi_write_byte(0);
            for(rc=0x80,rb=0;rb<8;rb++,rc>>=1)
            {
                if(font[d][ra>>3]&rc) rd=0xFF;
                else rd=0;
                spi_write_byte(rd);
            }
            spi_write_byte(0);
            for(rc=0x80,rb=0;rb<8;rb++,rc>>=1)
            {
                if(font[d][ra>>3]&rc) rd=0xFF;
                else rd=0;
                spi_write_byte(rd);
            }
            spi_write_byte(0x00);
            spi_write_byte(0x00);
            spi_delay();
            SPI_CS_OFF;
            spi_delay();
        }
        for(ra=0;ra<64;ra++)
        {
            spi_delay();
            SPI_CS_ON;
            spi_delay();
            spi_write_byte(0x80);
            spi_write_sharp_address((ra+94+1)&0xFF);
            spi_write_byte(0);
            for(rc=0x80,rb=0;rb<8;rb++,rc>>=1)
            {
                if(font[d][ra>>3]&rc) rd=0xFF;
                else rd=0;
                spi_write_byte(rd);
            }
            spi_write_byte(0);
            for(rc=0x80,rb=0;rb<8;rb++,rc>>=1)
            {
                if(font[d][ra>>3]&rc) rd=0xFF;
                else rd=0;
                spi_write_byte(rd);
            }
            spi_write_byte(0x00);
            spi_write_byte(0x00);
            spi_delay();
            SPI_CS_OFF;
            spi_delay();
        }


        for(ra=0;ra<1000000;ra++) dummy(ra);
        d++;
        if(d>10) d=0;
    }


    return(0);
}

/*

halt
flash write_image erase sharp02/notmain.elf
reset

*/


