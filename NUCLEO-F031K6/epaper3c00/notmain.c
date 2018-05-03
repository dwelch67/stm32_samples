
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



const unsigned char lut_vcom0[] =
{
    0x0E, 0x14, 0x01, 0x0A, 0x06, 0x04, 0x0A, 0x0A,
    0x0F, 0x03, 0x03, 0x0C, 0x06, 0x0A, 0x00
};

const unsigned char lut_w[] =
{
    0x0E, 0x14, 0x01, 0x0A, 0x46, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x86, 0x0A, 0x04
};

const unsigned char lut_b[] =
{
    0x0E, 0x14, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x06, 0x4A, 0x04
};

const unsigned char lut_g1[] =
{
    0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04
};

const unsigned char lut_g2[] =
{
    0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04
};

const unsigned char lut_vcom1[] =
{
    0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char lut_red0[] =
{
    0x83, 0x5D, 0x01, 0x81, 0x48, 0x23, 0x77, 0x77,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char lut_red1[] =
{
    0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

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
            break;
        }
        else
        {
            PUT32(GPIOBBASE+0x18,(1<<(3+16)));
        }
    }
}

static void spi_delay ( void )
{
    //unsigned int ra;
    //for(ra=0;ra<5;ra++) dummy(ra);
}

static void spi_two_ms ( void )
{
    unsigned int ra;
    for(ra=0;ra<16000;ra++) dummy(ra);
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
    //unsigned int x;
    unsigned int ry;


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





    spi_command(0x01);
    spi_data(0x07);
    spi_data(0x00);
    spi_data(0x08);
    spi_data(0x00);
    spi_command(0x06);
    spi_data(0x07);
    spi_data(0x07);
    spi_data(0x07);
    spi_command(0x04);

    spi_busy();

    spi_command(0x00);
    spi_data(0xcf);
    spi_command(0x50);
    spi_data(0x17);
    spi_command(0x30);
    spi_data(0x39);
    spi_command(0x61);
    spi_data(0xC8);
    spi_data(0x00);
    spi_data(0xC8);
    spi_command(0x82);
    spi_data(0x0E);

    spi_command(0x20);
    for(ra=0;ra<15;ra++)
    {
        spi_data(lut_vcom0[ra]);
    }
    spi_command(0x21);
    for(ra=0;ra<15;ra++)
    {
        spi_data(lut_w[ra]);
    }
    spi_command(0x22);
    for(ra=0;ra<15;ra++)
    {
        spi_data(lut_b[ra]);
    }
    spi_command(0x23);
    for(ra=0;ra<15;ra++)
    {
        spi_data(lut_g1[ra]);
    }
    spi_command(0x24);
    for(ra=0;ra<15;ra++)
    {
        spi_data(lut_g2[ra]);
    }
    spi_command(0x25);
    for(ra=0;ra<15;ra++)
    {
        spi_data(lut_vcom1[ra]);
    }
    spi_command(0x26);
    for(ra=0;ra<15;ra++)
    {
        spi_data(lut_red0[ra]);
    }
    spi_command(0x27);
    for(ra=0;ra<15;ra++)
    {
        spi_data(lut_red1[ra]);
    }



if(0)
{

    spi_command(0x10);
    spi_two_ms();

    for(ry=0;ry<2000;ry++) spi_data(0xFF);
    for(ry=0;ry<2000;ry++) spi_data(0x00);
    spi_two_ms();

    spi_command(0x13);
    spi_two_ms();
    for(ry=0;ry<1000;ry++) spi_data(0x00);
    for(ry=0;ry<1000;ry++) spi_data(0xFF);
    for(ry=0;ry<1000;ry++) spi_data(0x00);
    for(ry=0;ry<1000;ry++) spi_data(0xFF);
    for(ry=0;ry<1000;ry++) spi_data(0x00);
    spi_two_ms();
    spi_command(0x12);
    spi_busy();
}

if(0)
{

    spi_command(0x10);
    spi_two_ms();

    for(ry=0;ry<2000;ry++) spi_data(0xFF);
    for(ry=0;ry<2000;ry++) spi_data(0x00);
    spi_two_ms();

    spi_command(0x13);
    spi_two_ms();
    for(ry=0;ry<5000;ry++) spi_data(0xFF);
    spi_two_ms();

    spi_command(0x12);
    spi_busy();
}

if(0)
{

    spi_command(0x10);
    spi_two_ms();
    for(ry=0;ry<10000;ry++) spi_data(0xFF);
    spi_two_ms();

    spi_command(0x13);
    spi_two_ms();
    for(ry=0;ry<1000;ry++) spi_data(0x00);
    for(ry=0;ry<1000;ry++) spi_data(0xFF);
    for(ry=0;ry<1000;ry++) spi_data(0x00);
    for(ry=0;ry<1000;ry++) spi_data(0xFF);
    for(ry=0;ry<1000;ry++) spi_data(0x00);
    spi_two_ms();

    spi_command(0x12);
    spi_busy();
}

if(0)
{

    spi_command(0x10);
    spi_two_ms();
    for(ry=0;ry<10000;ry++) spi_data(0xFF);
    spi_two_ms();

    spi_command(0x13);
    spi_two_ms();
    for(ry=0;ry<12;ry++)
    {
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<25;rx++)
        {
            if(rx&1) spi_data(0xFF);
            else spi_data(0x00);
        }
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<25;rx++)
        {
            if(rx&1) spi_data(0x00);
            else spi_data(0xFF);
        }
    }
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<25;rx++)
        {
            if(rx&1) spi_data(0xFF);
            else spi_data(0x00);
        }
    spi_two_ms();

    spi_command(0x12);
    spi_busy();
}

if(0)
{

    spi_command(0x10);
    spi_two_ms();
    for(ry=0;ry<12;ry++)
    {
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<50;rx++)
        {
            if(rx&2) spi_data(0xFF);
            else spi_data(0x00);
        }
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<50;rx++)
        {
            if(rx&2) spi_data(0x00);
            else spi_data(0xFF);
        }
    }
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<50;rx++)
        {
            if(rx&2) spi_data(0xFF);
            else spi_data(0x00);
        }
    spi_two_ms();

    spi_command(0x13);
    spi_two_ms();
    for(ry=0;ry<5000;ry++) spi_data(0xFF);
    spi_two_ms();

    spi_command(0x12);
    spi_busy();
}

if(0)
{

    spi_command(0x10);
    spi_two_ms();
    for(ry=0;ry<12;ry++)
    {
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<50;rx++)
        {
            if(rx&2) spi_data(0xFF);
            else spi_data(0x00);
        }
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<50;rx++)
        {
            if(rx&2) spi_data(0x00);
            else spi_data(0xFF);
        }
    }
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<50;rx++)
        {
            if(rx&2) spi_data(0xFF);
            else spi_data(0x00);
        }
    spi_two_ms();

    spi_command(0x13);
    spi_two_ms();
    for(ry=0;ry<12;ry++)
    {
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<25;rx++)
        {
            if(rx&1) spi_data(0x00);
            else spi_data(0xFF);
        }
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<25;rx++)
        {
            if(rx&1) spi_data(0xFF);
            else spi_data(0x00);
        }
    }
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<25;rx++)
        {
            if(rx&1) spi_data(0x00);
            else spi_data(0xFF);
        }
    spi_two_ms();

    spi_command(0x12);
    spi_busy();
}

if(1)
{
    spi_command(0x10);
    spi_two_ms();
    for(ry=0;ry<12;ry++)
    {
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<50;rx++)
        {
            if(rx&2) spi_data(0xFF);
            else spi_data(0x00);
        }
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<50;rx++)
        {
            if(rx&2) spi_data(0x00);
            else spi_data(0xFF);
        }
    }
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<50;rx++)
        {
            if(rx&2) spi_data(0xFF);
            else spi_data(0x00);
        }
    spi_two_ms();

    spi_command(0x13);
    spi_two_ms();
    for(ry=0;ry<12;ry++)
    {
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<25;rx++)
        {
            if((rx&3)==3) spi_data(0x00);
            else spi_data(0xFF);
        }
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<25;rx++)
        {
            if((rx&3)==3) spi_data(0x00);
            else spi_data(0x00);
        }
    }
        for(ra=0;ra<8;ra++)
        for(rx=0;rx<25;rx++)
        {
            if((rx&3)==3) spi_data(0x00);
            else spi_data(0xFF);
        }
    spi_two_ms();

    spi_command(0x12);
    spi_busy();
}


if(0)
{
    spi_command(0x10);
    spi_two_ms();
    for(ry=0;ry<25;ry++)
    {
        for(rx=0;rx<200;rx++)
        {
            spi_data(0xFF);
        }
        for(rx=0;rx<200;rx++)
        {
            spi_data(0x00);
        }
    }
    spi_two_ms();

    spi_command(0x13);
    spi_two_ms();
    for(ry=0;ry<5000;ry++) spi_data(0xFF);
    spi_two_ms();


    spi_command(0x12);
    spi_busy();
}



    spi_command(0x50);
    spi_data(0x17);
    spi_command(0x82);
    spi_data(0x00);
    spi_command(0x01);
    spi_data(0x02);
    spi_data(0x00);
    spi_data(0x00);
    spi_data(0x00);
    spi_busy();
    spi_command(0x02);

    while(1)
    {
        PUT32(GPIOBBASE+0x18,(1<<3));
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOBBASE+0x18,(1<<(3+16)));
        for(ra=0;ra<200000;ra++) dummy(ra);
    }



    return(0);
}
