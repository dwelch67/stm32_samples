

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
#define USART1BASE  0x40013800
#define SPI1BASE    0x40013000

void clock_init ( void )
{
    unsigned int ra;

    //HSI8 based 20MHz.  (8/2)*5
    ra=0;
    ra|=0x3<<18; //times 5
    ra|=0<<15;  //divide by 2
    PUT32(RCCBASE+0x04,ra);
    ra=GET32(RCCBASE+0x00);
    ra|=1<<24;
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x00);
        if(ra&(1<<25)) break;
    }
    ra=GET32(RCCBASE+0x04);
    ra&=~3;
    ra|=2;
    PUT32(RCCBASE+0x04,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x04);
        if((ra&3)==2) break;
    }
}

void fifo_wait ( void )
{
    while(1)
    {
        if((GET16(SPI1BASE+0x08)&(1<<12))==0) break;
    }
}
const unsigned short data[6]=
{
0x2492,
0x4924,
0x9249,
0x2712,
0x4900,
0,
};

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    clock_init();

//PA4 AF0 SPI1_NSS
//PA5 AF0 SPI1_SCK
//PA6 AF0 SPI1_MISO
//PA7 AF0 SPI1_MOSI <---

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    //moder
    ra=GET32(GPIOABASE+0x00);
    //ra&=~(3<<(4<<1)); //PA4
    //ra|= (2<<(4<<1)); //PA4 alternate function
    //ra&=~(3<<(5<<1)); //PA5
    //ra|= (2<<(5<<1)); //PA5 alternate function
    //ra&=~(3<<(6<<1)); //PA6
    //ra|= (2<<(6<<1)); //PA6 alternate function
    ra&=~(3<<(7<<1)); //PA7
    ra|= (2<<(7<<1)); //PA7 alternate function
    PUT32(GPIOABASE+0x00,ra);
    //AFRL
    ra=GET32(GPIOABASE+0x20);
    //ra&=~(0xF<<(4<<2)); //PA4
    //ra|= (0x0<<(4<<2)); //PA4
    //ra&=~(0xF<<(5<<2)); //PA5
    //ra|= (0x0<<(5<<2)); //PA5
    //ra&=~(0xF<<(6<<2)); //PA6
    //ra|= (0x0<<(6<<2)); //PA6
    ra&=~(0xF<<(7<<2)); //PA7
    ra|= (0x0<<(7<<2)); //PA7
    PUT32(GPIOABASE+0x20,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<12; //enable spi1
    PUT32(RCCBASE+0x18,ra);

    //PUT32(RCCBASE+0x04,(0xF<<4)|(7<<8)); //SLOOOWWWW

    PUT16(SPI1BASE+0x00,(0<<6)|(2<<3)|(1<<2)|0);
    PUT16(SPI1BASE+0x04,0x0F0C);
    PUT16(SPI1BASE+0x00,(1<<6)|(2<<3)|(1<<2)|0);

//1101101101101101
//1011011011011011
//0110110110110110
//1101101101101101
//1011011000000000

//1101 1011 0110 1101 DB6D
//1011 0110 1101 1011 B6Db
//0110 1101 1011 0110 6DB6
//1101 1011 0110 1101 DB6D
//1011 0110 0000 0000 B600

    ra=0;
    PUT16(SPI1BASE+0x0C,data[ra++]);
    PUT16(SPI1BASE+0x0C,data[ra++]);
    //PUT16(SPI1BASE+0x0C,data[ra++]);
    for(ra=0;data[ra];ra++)
    {
        fifo_wait();
        PUT16(SPI1BASE+0x0C,data[ra]);
    }

    //for(rb=0;rb<6;rb++)
    //{
        //while(1)
        //{
            //ra=GET16(SPI1BASE+0x08);
            //if((ra&(1<<12))==0) break;
        //}
        //PUT8(SPI1BASE+0x0C,0xAA);
        ////GET8(SPI1BASE+0x0C);
        //while(1)
        //{
            //ra=GET16(SPI1BASE+0x08);
            //if((ra&(1<<12))==0) break;
        //}
        //PUT8(SPI1BASE+0x0C,0xCC);
        ////GET8(SPI1BASE+0x0C);
    //}
    //while(1)
    //{
        //ra=GET16(SPI1BASE+0x08);
        //if((ra&(1<<7))==0) break;
    //}

    return(0);
}


