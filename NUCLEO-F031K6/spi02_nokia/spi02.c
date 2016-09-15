
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#include "fontdata.h"


#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

//PA7 DIN
//PA5 CLK
//PA4 CE
//PA3 D/C

#define SET_MOSI    (1<<(7+0))
#define SET_CLK     (1<<(5+0))
#define SET_CS      (1<<(4+0))
#define SET_DC      (1<<(3+0))
#define RESET_MOSI  (1<<(7+16))
#define RESET_CLK   (1<<(5+16))
#define RESET_CS    (1<<(4+16))
#define RESET_DC    (1<<(3+16))

static void spi_delay ( void )
{
    unsigned int ra;
    for(ra=0;ra<5;ra++) dummy(ra);
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

static void show_string ( unsigned int row, const char *s )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;

    row&=7;
    spi_command(0x80); //column
    spi_command(0x40|row); //row
    rc=0;
    for(ra=0;ra<11;ra++)
    {
        if(s[ra]==0) break;
        for(rb=0;rb<8;rb++)
        {
            rd=s[ra];
            spi_data(fontdata[rd][rb]);
            rc++;
        }
    }
    for(;rc<84;rc++) spi_data(0);
}
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    
//PA7 DIN
//PA5 CLK
//PA4 CE
//PA3 D/C

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<14); //PA7
    ra&=~(3<<10); //PA5
    ra&=~(3<<8); //PA4
    ra&=~(3<<6); //PA3
    ra|=1<<14; //PA7
    ra|=1<<10; //PA5
    ra|=1<<8; //PA4
    ra|=1<<6; //PA3
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<7); //PA7
    ra&=~(1<<5); //PA5
    ra&=~(1<<4); //PA4
    ra&=~(1<<3); //PA3
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<14; //PA7
    ra|=3<<10; //PA5
    ra|=3<<8; //PA4
    ra|=3<<6; //PA3
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<14); //PA7
    ra&=~(3<<10); //PA5
    ra&=~(3<<8); //PA4
    ra&=~(3<<6); //PA3
    PUT32(GPIOABASE+0x0C,ra);

    spi_clk(0);
    spi_data(0);
    spi_cs(0);
    spi_cs(1);

    
    spi_command(0x21); //extended commands
//    spi_command(0xB0); //vop less contrast
    spi_command(0xBF); //vop more contrast
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

//experiment 1
if(1)
{
    spi_command(0x80); //column
    spi_command(0x40); //row
    for(ra=0;ra<504;ra++) spi_data(ra);
}

//experiment 2
if(1)
{
    spi_command(0x80); //column
    spi_command(0x40); //row
    for(ra=0;ra<10;ra++)
    {
        for(rb=0;rb<8;rb++) spi_data(fontdata[ra][rb]);
    }
    show_string(1,"Hello");
    show_string(2,"World");
}

////experiment 3
////4 boxes wide a clock would have three full digits plus a 1
////2 boxes wide for the 1, 4 each for the other three 2+4+4+$ = 14
////84/14 = 6 so six pixes wide per box...(8 tall).
//if(1)
//{
    //for(ra=0;ra<10;ra++)
    //{
        //for(rb=0;rb<5;rb++)
        //{
            //spi_command(0x80); //column
            //spi_command(0x40|rb); //row
            //rc=clockdata[ra][rb];
            //for(rd=0x8;rd;rd>>=1)
            //{
                //if(rc&rd) re=0xFF;
                //else      re=0x00;
                //for(rf=0;rf<6;rf++) spi_data(re);
            //}
        //}
        //for(rf=0;rf<0x100000;rf++) dummy(rf);
    //}
//}

    
    return(0);
}
