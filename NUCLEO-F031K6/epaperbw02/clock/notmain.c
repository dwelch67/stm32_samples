
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400

#define RCCBASE 0x40021000
#define USART1BASE 0x40013800

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
static unsigned int aa,bb,cc,dd;

static unsigned char num[8];
static unsigned int full;
void show_number ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rx;
    //unsigned int x;
    unsigned int y;


    display_init(full);

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

    if(num[0]==0)
    {
        for(y=0;y<32;y++)
        {
            for(ra=0;ra<32;ra++) spi_data(0xFF);
        }
    }
    else
    {
        for(ra=datastart[num[0]];fontdata[ra];ra+=2)
        {
            rb=fontdata[ra+0];
            rc=fontdata[ra+1];
            for(rx=0;rx<rb;rx++) spi_data(rc);
        }
    }
    for(y=0;y<5;y++)
    {
        for(ra=0;ra<32;ra++) spi_data(0xFF);
    }
    for(ra=datastart[num[1]];fontdata[ra];ra+=2)
    {
        rb=fontdata[ra+0];
        rc=fontdata[ra+1];
        for(rx=0;rx<rb;rx++) spi_data(rc);
    }
    for(y=0;y<10;y++)
    {
        for(ra=0;ra<32;ra++) spi_data(0xFF);
    }
    for(ra=datastart[num[2]];fontdata[ra];ra+=2)
    {
        rb=fontdata[ra+0];
        rc=fontdata[ra+1];
        for(rx=0;rx<rb;rx++) spi_data(rc);
    }
    for(y=0;y<5;y++)
    {
        for(ra=0;ra<32;ra++) spi_data(0xFF);
    }
    for(ra=datastart[num[3]];fontdata[ra];ra+=2)
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


void ten_div ( unsigned int ra )
{
    unsigned int num;
    unsigned int res;
    unsigned int acc;
    unsigned int den;
    unsigned int rb;


    num=ra;
    res=0;
    acc=0;
    den=10;
    for(rb=0x80000000;rb;rb>>=1)
    {
        acc<<=1;
        if(rb&num) acc|=1;
        if(acc>=den)
        {
            acc-=den;
            res|=rb;
        }
    }
    //printf("%u %u %u ",ra,res,acc);
    aa=res;

    num=acc;
    res=0;
    acc=0;
    den=10;
    for(rb=0x80000000;rb;rb>>=1)
    {
        acc<<=1;
        if(rb&num) acc|=1;
        if(acc>=den)
        {
            acc-=den;
            res|=rb;
        }
    }
    bb=res;

    num=acc;
    res=0;
    acc=0;
    den=10;
    for(rb=0x40;rb;rb>>=1)
    {
        acc<<=1;
        if(rb&num) acc|=1;
        if(acc>=den)
        {
            acc-=den;
            res|=rb;
        }
    }
    cc=res;
    dd=acc;
}


//------------------------------------------------------------------------
static int uart_init ( void )
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

    PUT32(USART1BASE+0x0C,833); //8Mhz/9600
    //PUT32(USART1BASE+0x0C,5000); //48Mhz/9600
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}
//------------------------------------------------------------------------
//static void uart_send ( unsigned int x )
//{
    //while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    //PUT32(USART1BASE+0x28,x);
//}
//------------------------------------------------------------------------
static unsigned int uart_recv ( void )
{
    //while(1)
    //{
        //if(zhead!=ztail)
        //{
            //ztail=(ztail+1)&ZMASK;
            //return(zbuff[ztail]);
        //}
        //show_time();
    //}
    while(1)
    {
        if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    }
    return(GET32(USART1BASE+0x24));
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------

static unsigned char xstring[32];
static unsigned int tim[4];
static unsigned int lasttim[4];
//------------------------------------------------------------------------
static void show_clock ( void )
{
    unsigned int ra;

    ra=0;
    if(tim[0]!=lasttim[0]) ra++;
    if(tim[1]!=lasttim[1]) ra++;
    if(tim[2]!=lasttim[2]) ra++;
    if(tim[3]!=lasttim[3]) ra++;
    if(ra==0) return;


    if(tim[1]!=lasttim[1]) full=1; else full=0;

    lasttim[0]=tim[0];
    lasttim[1]=tim[1];
    lasttim[2]=tim[2];
    lasttim[3]=tim[3];

    num[0]=tim[0];
    num[1]=tim[1];
    num[2]=tim[2];
    num[3]=tim[3];

    show_number();
}
//$GPRMC,000143.00,A,4030.97866,N,07955.13947,W,0.419,,020416,,,A*6E

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
static unsigned int timezone;
//------------------------------------------------------------------------
static int do_nmea ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int state;
    unsigned int off;
    //unsigned char toggle_seconds;

    //toggle_seconds=0;
    state=0;
    off=0;
//$GPRMC,054033.00,V,
    while(1)
    {
        ra=uart_recv();
        //uart_send(ra);
        //uart_send(0x30+state);
        switch(state)
        {
            case 0:
            {
                if(ra=='$') state++;
                else state=0;
                break;
            }
            case 1:
            {
                if(ra=='G') state++;
                else state=0;
                break;
            }
            case 2:
            {
                if(ra=='N') state++;
                else state=0;
                break;
            }
            case 3:
            {
                if(ra=='R') state++;
                else state=0;
                break;
            }
            case 4:
            {
                if(ra=='M') state++;
                else state=0;
                break;
            }
            case 5:
            {
                if(ra=='C') state++;
                else state=0;
                break;
            }
            case 6:
            {
                off=0;
                if(ra==',') state++;
                else state=0;
                break;
            }
            case 7:
            {
                if(ra==',')
                {
                    if(off>7)
                    {
                        rb=xstring[0]&0xF;
                        rc=xstring[1]&0xF;
                        //1010
                        rb=/*rb*10*/(rb<<3)+(rb<<1); //times 10
                        rb+=rc;
                        if(rb>12) rb-=12;
                        //ra=5; //time zone adjustment winter
                        //ra=4; //time zone adjustment summer
                        ra=timezone;
                        if(rb<=ra) rb+=12;
                        rb-=ra;
                        if(rb>9)
                        {
                            xstring[0]='1';
                            rb-=10;
                        }
                        else
                        {
                            xstring[0]='0';
                        }
                        rb&=0xF;
                        xstring[1]=0x30+rb;
                        rb=0;
                        //zstring[rb++]=0x77;
                        //toggle_seconds^=0x10;
                        //zstring[rb++]=toggle_seconds;
                        //zstring[rb++]=xstring[0];
                        //zstring[rb++]=xstring[1];
                        //zstring[rb++]=xstring[2];
                        //zstring[rb++]=xstring[3];
                        //xstring[rb++]=0x0D;
                        //zstring[rb++]=0;
                        tim[0]=xstring[0]&0xF;
                        tim[1]=xstring[1]&0xF;
                        tim[2]=xstring[2]&0xF;
                        tim[3]=xstring[3]&0xF;
                    }
                    else
                    {
                        //zstring[0]=0x33;
                        //zstring[1]=0x33;
                        //zstring[2]=0x33;
                        //zstring[3]=0x33;
                        //xstring[4]=0x0D;
                        //zstring[5]=0;
                        tim[0]=3;
                        tim[1]=3;
                        tim[2]=3;
                        tim[3]=3;
                    }
                    off=0;
                    state++;
                }
                else
                {
                    if(off<16)
                    {
                        xstring[off++]=ra;
                    }
                }
                break;
            }
            case 8:
            {
                //if(zstring[off]==0)
                //{
                    //state=0;
                //}
                //else
                //{
                    //uart_send(zstring[off++]);
                //}
                //show_time();
                show_clock();
                state=0;
                break;
            }
        }
    }
    return(0);
}





int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    num[0]=3;
    num[1]=4;
    num[2]=5;
    num[3]=6;
    num[4]=0;
    num[5]=0;


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

    uart_init();

    timezone=4;
    rb=0x44444444;
    ra=GET32(0x20000D00);
    if(ra==0x44444444)
    {
        timezone=5; //EST
        rb=0x55555555;
    }
    else
    if(ra==0x55555555)
    {
        timezone=4; //EDT
        rb=0x44444444;
    }
    PUT32(0x20000D00,rb);


    full=0;

    tim[0]=1;
    tim[1]=2;
    tim[2]=3;
    tim[3]=4;

    lasttim[0]=0;
    lasttim[1]=0;
    lasttim[2]=0;
    lasttim[3]=0;

    show_clock();

    do_nmea();

    return(0);
}
