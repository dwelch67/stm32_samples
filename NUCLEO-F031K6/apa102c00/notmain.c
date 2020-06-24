

#define DEBUGUART

void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8  ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET8 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE   0x48000000
#define RCCBASE     0x40021000
#define SPI1BASE    0x40013000

static void fifo_wait ( void )
{
    while(1)
    {
        if((GET16(SPI1BASE+0x08)&(1<<12))==0) break;
    }
}

unsigned short data[512];
unsigned int off;

static void run_it ( void )
{
    unsigned int ra;

    PUT16(SPI1BASE+0x0C,0x0000); //extra zeros are okay.
    for(ra=0;ra<off;ra++)
    {
        fifo_wait();
        PUT16(SPI1BASE+0x0C,data[ra]);
    }
    //PUT16(SPI1BASE+0x0C,0x0000);
    //for(ra=0;ra<40000;ra++) dummy(ra);
}

int notmain ( void )
{
    unsigned int ra;

//PA4 AF0 SPI1_NSS
//PA5 AF0 SPI1_SCK  <---
//PA6 AF0 SPI1_MISO
//PA7 AF0 SPI1_MOSI <---

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    //MODER
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(5<<1)); //PA5
    ra|= (2<<(5<<1)); //PA5 alternate function
    ra&=~(3<<(7<<1)); //PA7
    ra|= (2<<(7<<1)); //PA7 alternate function
    PUT32(GPIOABASE+0x00,ra);
    //AFRL
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<(5<<2)); //PA5
    ra|= (0x0<<(5<<2)); //PA5
    ra&=~(0xF<<(7<<2)); //PA7
    ra|= (0x0<<(7<<2)); //PA7
    PUT32(GPIOABASE+0x20,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<12; //enable spi1
    PUT32(RCCBASE+0x18,ra);

    PUT16(SPI1BASE+0x00,(0<<6)|(2<<3)|(1<<2)|3);
    PUT16(SPI1BASE+0x04,0x0F04);
    PUT16(SPI1BASE+0x00,(1<<6)|(2<<3)|(1<<2)|3);

    off=0;
    data[off++]=0x0000;  data[off++]=0x0000;

    data[off++]=0xE101;  data[off++]=0x0000;
    data[off++]=0xE100;  data[off++]=0x0100;
    data[off++]=0xE100;  data[off++]=0x0001;

    data[off++]=0xE102;  data[off++]=0x0000;
    data[off++]=0xE100;  data[off++]=0x0200;
    data[off++]=0xE100;  data[off++]=0x0002;

    data[off++]=0xE401;  data[off++]=0x0100;
    data[off++]=0xE401;  data[off++]=0x0001;
    data[off++]=0xE400;  data[off++]=0x0101;

    data[off++]=0xFFFF;  data[off++]=0xFFFF;
    run_it();

    return(0);
}


