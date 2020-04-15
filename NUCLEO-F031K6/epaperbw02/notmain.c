
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400

#define RCCBASE 0x40021000

//PA7 DIN  OUT
//PA5 CLK  OUT
//PA4 CS   OUT
//PA3 DC   OUT
//PA1 RST  OUT
//PA0 BUSY in

#define SET_MOSI    (1<<(7+0))
#define SET_CLK     (1<<(5+0))
#define SET_CS      (1<<(4+0))
#define SET_DC      (1<<(3+0))
#define SET_RST     (1<<(1+0))
#define RESET_MOSI  (1<<(7+16))
#define RESET_CLK   (1<<(5+16))
#define RESET_CS    (1<<(4+16))
#define RESET_DC    (1<<(3+16))
#define RESET_RST   (1<<(1+16))
#define BUSY_BIT (1<<0)

//old, why was it different then?
//const unsigned char lut_full_update[] =
//{
    //0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
    //0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
    //0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
    //0x35, 0x51, 0x51, 0x19, 0x01, 0x00
//};

const unsigned char lut_full_update[] =
{
    0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char lut_partial_update[] =
{
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#include "data.h"

static void spi_reset ( void )
{
    unsigned int ra;
    PUT32(GPIOABASE+0x18,RESET_RST);
    for(ra=0;ra<50000;ra++) dummy(ra);
    PUT32(GPIOABASE+0x18,SET_RST);
    for(ra=0;ra<50000;ra++) dummy(ra);
}

static void spi_busy ( void )
{
    while(1)
    {
        if(GET32(GPIOABASE+0x10)&1)
        {
            PUT32(GPIOBBASE+0x18,(1<<(3+ 0)));
        }
        else
        {
            PUT32(GPIOBBASE+0x18,(1<<(3+16)));
            break;
        }
    }
}

static void spi_delay ( void )
{
    //unsigned int ra;
    //for(ra=0;ra<5;ra++) dummy(ra);
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

static void display_init ( unsigned int full )
{
    unsigned int ra;

    spi_reset();

    //spi_command(0x12);
    //for(ra=0;ra<20000;ra++) dummy(ra); //is this right or a busy check?
    //spi_busy();

    //Driver Output control
    spi_command(0x01);
    spi_data(0x27);
    spi_data(0x01);
    spi_data(0x00);
    //Booster Soft Control Start
    spi_command(0x0C);
    spi_data(0xCF);
    spi_data(0xCE);
    spi_data(0x8D);

    //VCOM
    spi_command(0x2C);
    spi_data(0xA8);
    //set dummy line period
    spi_command(0x3A);
    spi_data(0x1A);
    //set line gate time
    spi_command(0x3B);
    spi_data(0x08);

    //border Waveform Control ??
    spi_command(0x3C);
    spi_data(0x03);

    //data entry mode setting
    spi_command(0x11);
    spi_data(0x03);

    if(full)
    {
        //lut table
        spi_command(0x32);
        for(ra=0;ra<30;ra++)
        {
            spi_data(lut_full_update[ra]);
        }
    }
    else
    {
        //lut table
        spi_command(0x32);
        for(ra=0;ra<30;ra++)
        {
            spi_data(lut_partial_update[ra]);
        }
    }
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rx;
    //unsigned int x;
    unsigned int y;


    ra=GET32(RCCBASE+0x14);
    ra|=1<<18; //enable port b
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOBBASE+0x00);
    ra&=~(3<<6); //PB3
    ra|=1<<6; //PB3
    PUT32(GPIOBBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOBBASE+0x04);
    ra&=~(1<<3); //PB3
    PUT32(GPIOBBASE+0x04,ra);
//    PUT32(GPIOBBASE+0x18,(1<<3));




//PA7 DIN  OUT
//PA5 CLK  OUT
//PA4 CS   OUT
//PA3 DC   OUT
//PA1 RST  OUT
//PA0 BUSY in

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<14); //PA7
    ra&=~(3<<10); //PA5
    ra&=~(3<<8); //PA4
    ra&=~(3<<6); //PA3
    ra&=~(3<<2); //PA1
    ra&=~(3<<0); //PA0
    ra|=1<<14; //PA7
    ra|=1<<10; //PA5
    ra|=1<<8; //PA4
    ra|=1<<6; //PA3
    ra|=1<<2; //PA1
    //ra|=0<<6; //PA0
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<7); //PA7
    ra&=~(1<<5); //PA5
    ra&=~(1<<4); //PA4
    ra&=~(1<<3); //PA3
    ra&=~(1<<1); //PA1
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<14; //PA7
    ra|=3<<10; //PA5
    ra|=3<<8; //PA4
    ra|=3<<6; //PA3
    ra|=3<<2; //PA1
    ra|=3<<0; //PA0
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<14); //PA7
    ra&=~(3<<10); //PA5
    ra&=~(3<<8); //PA4
    ra&=~(3<<6); //PA3
    ra&=~(3<<2); //PA1
    ra&=~(3<<0); //PA0
    ra|= (1<<0); //PA0
    PUT32(GPIOABASE+0x0C,ra);

    spi_clk(0);
    spi_data(0);
    spi_cs(0);
    spi_cs(1);

    spi_reset();




    display_init(1);

//for(rx=0;rx<2;rx++)
{

    //set x start and end
    spi_command(0x44);
    spi_data((0 >> 3) & 0xFF); //x start
    spi_data(((128-1) >> 3) & 0xFF); //x end
    //set y start and end
    spi_command(0x45);
    spi_data(0 & 0xFF); //y start
    spi_data((0 >> 8) & 0xFF); //y start
    spi_data((296-1) & 0xFF); //y end
    spi_data(((296-1) >> 8) & 0xFF); //y end

    //ram x address counter
    spi_command(0x4E);
    spi_data(0x00);
    //ram y address counter
    spi_command(0x4F);
    spi_data(0x00);
    spi_data(0x00);
    spi_busy();

    //write ram
    spi_command(0x24);
    //for(y=0;y<37;y++)
    //{
        //for(ra=0;ra<64;ra++) spi_data(0xF0);
        //for(ra=0;ra<64;ra++) spi_data(0x0F);
    //}
    for(ra=datastart[0];fontdata[ra];ra+=2)
    {
        rb=fontdata[ra+0];
        rc=fontdata[ra+1];
        for(rx=0;rx<rb;rx++) spi_data(rc);
    }
    for(y=0;y<5;y++)
    {
        for(ra=0;ra<32;ra++) spi_data(0xFF);
    }
    for(ra=datastart[1];fontdata[ra];ra+=2)
    {
        rb=fontdata[ra+0];
        rc=fontdata[ra+1];
        for(rx=0;rx<rb;rx++) spi_data(rc);
    }
    for(y=0;y<10;y++)
    {
        for(ra=0;ra<32;ra++) spi_data(0xFF);
    }
    for(ra=datastart[2];fontdata[ra];ra+=2)
    {
        rb=fontdata[ra+0];
        rc=fontdata[ra+1];
        for(rx=0;rx<rb;rx++) spi_data(rc);
    }
    for(y=0;y<5;y++)
    {
        for(ra=0;ra<32;ra++) spi_data(0xFF);
    }
    for(ra=datastart[3];fontdata[ra];ra+=2)
    {
        rb=fontdata[ra+0];
        rc=fontdata[ra+1];
        for(rx=0;rx<rb;rx++) spi_data(rc);
    }


    spi_command(0x22);
    spi_data(0xC4);
    spi_command(0x20);
    spi_command(0xFF);
    spi_busy();

    //put to sleep
    spi_command(0x10);
    spi_data(0x01);

}


        for(ra=0;ra<200000;ra++) dummy(ra);



    display_init(1);

//for(rx=0;rx<2;rx++)
{

    //set x start and end
    spi_command(0x44);
    spi_data((0 >> 3) & 0xFF); //x start
    spi_data(((128-1) >> 3) & 0xFF); //x end
    //set y start and end
    spi_command(0x45);
    spi_data(0 & 0xFF); //y start
    spi_data((0 >> 8) & 0xFF); //y start
    spi_data((296-1) & 0xFF); //y end
    spi_data(((296-1) >> 8) & 0xFF); //y end

    //ram x address counter
    spi_command(0x4E);
    spi_data(0x00);
    //ram y address counter
    spi_command(0x4F);
    spi_data(0x00);
    spi_data(0x00);
    spi_busy();

    //write ram
    spi_command(0x24);
    //for(y=0;y<37;y++)
    //{
        //for(ra=0;ra<64;ra++) spi_data(0xF0);
        //for(ra=0;ra<64;ra++) spi_data(0x0F);
    //}
    for(ra=datastart[4];fontdata[ra];ra+=2)
    {
        rb=fontdata[ra+0];
        rc=fontdata[ra+1];
        for(rx=0;rx<rb;rx++) spi_data(rc);
    }
    for(y=0;y<5;y++)
    {
        for(ra=0;ra<32;ra++) spi_data(0xFF);
    }
    for(ra=datastart[5];fontdata[ra];ra+=2)
    {
        rb=fontdata[ra+0];
        rc=fontdata[ra+1];
        for(rx=0;rx<rb;rx++) spi_data(rc);
    }
    for(y=0;y<10;y++)
    {
        for(ra=0;ra<32;ra++) spi_data(0xFF);
    }
    for(ra=datastart[6];fontdata[ra];ra+=2)
    {
        rb=fontdata[ra+0];
        rc=fontdata[ra+1];
        for(rx=0;rx<rb;rx++) spi_data(rc);
    }
    for(y=0;y<5;y++)
    {
        for(ra=0;ra<32;ra++) spi_data(0xFF);
    }
    for(ra=datastart[7];fontdata[ra];ra+=2)
    {
        rb=fontdata[ra+0];
        rc=fontdata[ra+1];
        for(rx=0;rx<rb;rx++) spi_data(rc);
    }


    spi_command(0x22);
    spi_data(0xC4);
    spi_command(0x20);
    spi_command(0xFF);
    spi_busy();

    //put to sleep
    spi_command(0x10);
    spi_data(0x01);

}


    //display_init(0);


//for(rx=0;rx<4;rx++)
//{

    ////set x start and end
    //spi_command(0x44);
    //spi_data((0 >> 3) & 0xFF); //x start
    //spi_data(((128-1) >> 3) & 0xFF); //x end
    ////set y start and end
    //spi_command(0x45);
    //spi_data(0 & 0xFF); //y start
    //spi_data((0 >> 8) & 0xFF); //y start
    //spi_data((296-1) & 0xFF); //y end
    //spi_data(((296-1) >> 8) & 0xFF); //y end

    ////ram x address counter
    //spi_command(0x4E);
    //spi_data(0x00);
    ////ram y address counter
    //spi_command(0x4F);
    //spi_data(0x00);
    //spi_data(0x00);
    //spi_busy();

    ////write ram
    //spi_command(0x24);
//if(rx&1)
//{
    //for(y=0;y<37;y++)
    //{
        //for(ra=0;ra<64;ra++) spi_data(0xFF);
        //for(ra=0;ra<64;ra++) spi_data(0xFF);
    //}
//}
//else
//{
    //for(y=0;y<37;y++)
    //{
////        for(ra=0;ra<64;ra++) spi_data(0x00);
  ////      for(ra=0;ra<64;ra++) spi_data(0x00);
        //for(ra=0;ra<64;ra++)
        //{
            //spi_data(0x00);
            //spi_data(0xFF);
        //}
    //}
//}

    //spi_command(0x22);
    //spi_data(0xC4);
    //spi_command(0x20);
    //spi_command(0xFF);
    //spi_busy();

//}













    ////set x start and end
    //spi_command(0x44);
    //spi_data((0 >> 3) & 0xFF); //x start
    //spi_data(((128-1) >> 3) & 0xFF); //x end
    ////set y start and end
    //spi_command(0x45);
    //spi_data(0 & 0xFF); //y start
    //spi_data((0 >> 8) & 0xFF); //y start
    //spi_data((296-1) & 0xFF); //y end
    //spi_data(((296-1) >> 8) & 0xFF); //y end

    ////ram x address counter
    //spi_command(0x4E);
    //spi_data(0x00);
    ////ram y address counter
    //spi_command(0x4F);
    //spi_data(0x00);
    //spi_data(0x00);
    //spi_busy();

    ////write ram
    //spi_command(0x24);
    ////for(y=0;y<30;y++)
    ////{
        ////for(ra=0;ra<64;ra++) spi_data(0xFF);
        ////for(ra=0;ra<64;ra++) spi_data(0xFF);
    ////}
    ////for(y=0;y<7;y++)
    ////{
        ////for(ra=0;ra<64;ra++) spi_data(0x00);
        ////for(ra=0;ra<64;ra++) spi_data(0x00);
    ////}
        ////for(ra=0;ra<32;ra++) spi_data(0xAA);

    //for(y=0;y<sizeof(data);y++)
    //{
        //spi_data(data[y]);
    //}
    //for(y=0;y<4;y++)
    //{
        //for(ra=0;ra<32;ra++) spi_data(0x00);
    //}
    //for(y=0;y<sizeof(data);y++)
    //{
        //spi_data(data[y]);
    //}
    //for(y=0;y<4;y++)
    //{
        //for(ra=0;ra<32;ra++) spi_data(0x00);
    //}
    //for(y=0;y<sizeof(data);y++)
    //{
        //spi_data(data[y]);
    //}
    //for(y=0;y<4;y++)
    //{
        //for(ra=0;ra<32;ra++) spi_data(0x00);
    //}
    //for(y=0;y<sizeof(data);y++)
    //{
        //spi_data(data[y]);
    //}
    //for(y=0;y<4;y++)
    //{
        //for(ra=0;ra<32;ra++) spi_data(0x00);
    //}


    //spi_command(0x22);
    //spi_data(0xC4);
    //spi_command(0x20);
    //spi_command(0xFF);
    //spi_busy();

















    ////put to sleep
    //spi_command(0x10);
    //spi_data(0x01);
















    while(1)
    {
        PUT32(GPIOBBASE+0x18,(1<<3));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOBBASE+0x18,(1<<(3+16)));
        for(ra=0;ra<200000;ra++) dummy(ra);
    }



    return(0);
}
