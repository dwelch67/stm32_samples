
void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8  ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET8 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE   0x50000000

#define RCCBASE     0x40021000

#define SPI1BASE    0x40013000

#define STK_CSR     0xE000E010
#define STK_RVR     0xE000E014
#define STK_CVR     0xE000E018

static void clock_init ( void )
{
    unsigned int ra;

    //want 20MHz

    //f VCO = f PLLIN × (N / M)
    //f PLLP = f VCO / P
    //f PLLR = f VCO / R
    //Maximum VCO frequency is 344 MHz.  PCLK
    //Maximum VCO frequency is 128 MHz.  RCLK
    //rclk ->  sysclk

    //HSI16 source 16MHz
    //source/m between 4 and 16
    //m = 2
    //16/2 = 8
    //n = 10
    //vco = 16*(10/2) = 80
    //R = 4
    //pllr = 80/4 = 20MHz
    //make p the same not using it.

    ra=0;
    ra|=3<<29; //R = 4
    ra|=1<<28; //PLLR enable
    ra|=0<<16; //PLLP disable
    ra|=10<<8; //N = 10
    ra|=1<<4;  //M = 2
    ra|=2<<0;  //HSI16 source
    PUT32(RCCBASE+0x0C,ra);

    ra=GET32(RCCBASE+0x00);
    ra|=1<<24; //PLLON
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x00);
        if(ra&(1<<25)) break; //PLLRDY
    }

    ra=GET32(RCCBASE+0x08);
    ra&=~3;
    ra|= 2; //PLLRCLK
    PUT32(RCCBASE+0x08,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x08);
        if((ra&3)==2) break; //PLLRCLK
    }
}

static void fifo_wait ( void )
{
    while(1)
    {
        if((GET16(SPI1BASE+0x08)&(1<<12))==0) break;
    }
}

unsigned short data[32];
unsigned int off;
unsigned int bit;

static void add_bit ( unsigned int x )
{
    data[off]<<=1;
    data[off]|=(x&1);
    bit++;
    if(bit==16)
    {
        off++;
        bit=0;
    }
}

static void add_one_color ( unsigned int c )
{
    unsigned int ra;
    for(ra=0x80;ra;ra>>=1)
    {
        if(c&ra)
        {
            add_bit(1);
            add_bit(1);
            add_bit(0);
        }
        else
        {
            add_bit(1);
            add_bit(0);
            add_bit(0);
        }
    }
}

static void add_colors ( unsigned int g, unsigned int r, unsigned int b )
{
    add_one_color(g);
    add_one_color(r);
    add_one_color(b);
}

static void run_it ( void )
{
    unsigned int ra;

    PUT16(SPI1BASE+0x0C,0x0000);
    for(ra=0;ra<off;ra++)
    {
        fifo_wait();
        PUT16(SPI1BASE+0x0C,data[ra]);
    }
    PUT16(SPI1BASE+0x0C,0x0000);
    for(ra=0;ra<400000;ra++) dummy(ra);
}

//static void round_off ( void )
//{
            ////add_bit(1);
            //add_bit(0);
    //while(bit!=0) add_bit(0);
//}


int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    clock_init();

//PA4 AF0 SPI1_NSS
//PA5 AF0 SPI1_SCK
//PA6 AF0 SPI1_MISO
//PA7 AF0 SPI1_MOSI <---

    ra=GET32(RCCBASE+0x34);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x34,ra);

    //MODER
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(7<<1)); //PA7
    ra|= (2<<(7<<1)); //PA7 alternate function
    PUT32(GPIOABASE+0x00,ra);

    //AFRL
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<(7<<2)); //PA7
    ra|= (0x0<<(7<<2)); //PA7
    PUT32(GPIOABASE+0x20,ra);

    ra=GET32(RCCBASE+0x40);
    ra|=1<<12; //enable spi1
    PUT32(RCCBASE+0x40,ra);

    //20MHz / 8 = 2500000   0.4us
    PUT16(SPI1BASE+0x00,(0<<6)|(2<<3)|(1<<2)|0);
    PUT16(SPI1BASE+0x04,0x0F04);
    PUT16(SPI1BASE+0x00,(1<<6)|(2<<3)|(1<<2)|0);

//while(1)
//{
    //off=0;
    //bit=0;

    //data[off++]=0x0000;
    //add_colors(0x00,0x00,0x01);
    //add_colors(0x00,0x01,0x00);
    //add_colors(0x01,0x00,0x00);
    //add_colors(0x00,0x00,0x01);
    //if(bit==16)
    //{
        //off++;
        //bit=0;
    //}
    //while(bit!=0) add_bit(0);
    //data[off++]=0x0000;
    //run_it();

    //off=0;
    //bit=0;

    //data[off++]=0x0000;
    //add_colors(0x00,0x01,0x00);
    //add_colors(0x01,0x00,0x00);
    //add_colors(0x00,0x00,0x01);
    //add_colors(0x00,0x01,0x00);
    //if(bit==16)
    //{
        //off++;
        //bit=0;
    //}
    //while(bit!=0) add_bit(0);
    //data[off++]=0x0000;

    //run_it();

    //off=0;
    //bit=0;

    //data[off++]=0x0000;
    //add_colors(0x01,0x00,0x00);
    //add_colors(0x00,0x00,0x01);
    //add_colors(0x00,0x01,0x00);
    //add_colors(0x01,0x00,0x00);
    //if(bit==16)
    //{
        //off++;
        //bit=0;
    //}
    //while(bit!=0) add_bit(0);
    //data[off++]=0x0000;
    //run_it();

//}


for(rx=0;;rx++)
{
    off=0;
    bit=0;

    data[off++]=0x0000;
    //add_colors(0x00,((~rx)>>0)&1,(rx>>0)&1);
    //add_colors(0x00,((~rx)>>1)&1,(rx>>1)&1);
    //add_colors(0x00,((~rx)>>2)&1,(rx>>2)&1);
    //add_colors(0x00,((~rx)>>4)&1,(rx>>4)&1);

    add_colors(((rx)>>0)&1,((rx)>>0)&1,0x01);
    add_colors(((rx)>>1)&1,((rx)>>1)&1,0x01);
    add_colors(((rx)>>2)&1,((rx)>>2)&1,0x01);
    add_colors(((rx)>>4)&1,((rx)>>4)&1,0x01);



    //add_colors(0x00,(((~rx)>>0)&1)|((rx<<1)&2),0x00);
    //add_colors(0x00,(((~rx)>>1)&1)|((rx>>0)&2),0x00);
    //add_colors(0x00,(((~rx)>>2)&1)|((rx>>1)&2),0x00);
    //add_colors(0x00,(((~rx)>>4)&1)|((rx>>2)&2),0x00);

    if(bit==16)
    {
        off++;
        bit=0;
    }
    while(bit!=0) add_bit(0);
    data[off++]=0x0000;
    run_it();

}




    return(0);
}


