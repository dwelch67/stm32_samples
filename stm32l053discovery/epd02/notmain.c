
//------------------------------------------------------------------------
//------------------------------------------------------------------------

#include "fontdata.h"

void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void ASMDELAY ( unsigned int );

#define BSRR_SET 0
#define BSRR_RESET 16

#define USART1BASE  0x40013800

#define GPIOABASE   0x50000000
#define GPIOBBASE   0x50000400
#define RCCBASE     0x40021000
static const unsigned char WF_LUT[]={
  0x82,0x00,0x00,0x00,0xAA,0x00,0x00,0x00,
  0xAA,0xAA,0x00,0x00,0xAA,0xAA,0xAA,0x00,
  0x55,0xAA,0xAA,0x00,0x55,0x55,0x55,0x55,
  0xAA,0xAA,0xAA,0xAA,0x55,0x55,0x55,0x55,
  0xAA,0xAA,0xAA,0xAA,0x15,0x15,0x15,0x15,
  0x05,0x05,0x05,0x05,0x01,0x01,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x41,0x45,0xF1,0xFF,0x5F,0x55,0x01,0x00,
  0x00,0x00,};
static const unsigned char font[8]={0x1C,0x36,0x30,0x78,0x30,0x30,0x78,0x00};

//------------------------------------------------------------------------
static void clock_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x00);
    ra|=1<<0;
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x00);
        if(ra&(1<<2)) break;
    }
    PUT32(RCCBASE+0x0C,1);
}
//------------------------------------------------------------------------
static void uart_init ( void )
{
    unsigned int ra;

    //need HSI16 up
    //ra=GET32(RCCBASE+0x00);
    //ra|=1<<0;
    //PUT32(RCCBASE+0x00,ra);
    //while(1)
    //{
        //ra=GET32(RCCBASE+0x00);
        //if(ra&(1<<2)) break;
    //}

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x2C,ra);

    ra=GET32(RCCBASE+0x34);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x34,ra);

    ra=GET32(RCCBASE+0x4C);
    ra&=~(3<<0);
    ra|=2<<0; //HSI16 clock for uart
    PUT32(RCCBASE+0x4C,ra);


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
    //afr4 0100
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x4<<4; //PA9 AF4 USART1_TX
    ra&=~(0xF<<8); //PA10
    ra|=0x4<<8; //PA10 AF4 USART1_RX
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x24);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x24,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x24,ra);

    PUT32(USART1BASE+0x0C,139);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);
}
//------------------------------------------------------------------------
static void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
}
////------------------------------------------------------------------------
//static unsigned int uart_recv ( void )
//{
    //while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    //return(GET32(USART1BASE+0x24));
//}
//------------------------------------------------------------------------
static void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}
//------------------------------------------------------------------------
static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
static void gpio_init ( void )
{
    unsigned int ra;

    //PA8     BUSY
    //PA15    CS

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x2C,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<16);   //PA8
    ra&=~(3<<30);   //PA15
    ra|=1<<30;      //PA15
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<15);   //PA15
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<30;      //PA15
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<16);   //PA8
    ra|=2<<16;      //PA8 down
    ra&=~(3<<30);   //PA15
    PUT32(GPIOABASE+0x0C,ra);

    //PB2     RESET
    //PB3     SCK
    //PB5     MOSI
    //PB10    PWR_EN
    //PB11    D/C

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<1; //enable port b
    PUT32(RCCBASE+0x2C,ra);
    //moder
    ra=GET32(GPIOBBASE+0x00);
    ra&=~(3<<4);    //PB2
    ra|=1<<4;       //PB2
    ra&=~(3<<6);    //PB3
    ra|=1<<6;       //PB3
    ra&=~(3<<10);   //PB5
    ra|=1<<10;      //PB5
    ra&=~(3<<20);   //PB10
    ra|=1<<20;      //PB10
    ra&=~(3<<22);   //PB11
    ra|=1<<22;      //PB11
    PUT32(GPIOBBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOBBASE+0x04);
    ra&=~(1<<2);    //PB2
    ra&=~(1<<3);    //PB3
    ra&=~(1<<5);    //PB5
    ra&=~(1<<10);   //PB10
    ra&=~(1<<11);   //PB11
    PUT32(GPIOBBASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOBBASE+0x08);
    ra|=3<<4;       //PB2
    ra|=3<<6;       //PB3
    ra|=3<<10;      //PB5
    ra|=3<<20;      //PB10
    ra|=3<<22;      //PB11
    PUT32(GPIOBBASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOBBASE+0x0C);
    ra&=~(3<<4);    //PB2
    ra&=~(3<<6);    //PB3
    ra&=~(3<<10);   //PB5
    ra&=~(3<<20);   //PB10
    ra&=~(3<<22);   //PB11
    PUT32(GPIOBBASE+0x0C,ra);

}
//------------------------------------------------------------------------
static void delay ( void  )
{
    //unsigned int ra;

    //for(ra=0;ra<10;ra++) dummy(ra);
    //ASMDELAY(1000);
}
//------------------------------------------------------------------------

//PA8     BUSY
//PB2     RESET
//PB11    D/C
//PA15    CS
//PB3     SCK
//PB5     MOSI
//PB10    PWR_EN

#define GPIOABASE   0x50000000
#define GPIOBBASE   0x50000400
#define RCCBASE     0x40021000

////------------------------------------------------------------------------
//static void epd_power ( unsigned int on )
//{
    ////PB10    PWR_EN
    //if(on)
    //{
        //PUT32(GPIOBBASE+0x18,1<<(10+BSRR_SET));
    //}
    //else
    //{
        //PUT32(GPIOBBASE+0x18,1<<(10+BSRR_RESET));
    //}
//}
//------------------------------------------------------------------------
static void epd_busy_wait ( void )
{
    //PA8     BUSY
    //When Busy is High, the operation of chip should not be interrupted,
    while(1)
    {
        if((GET32(GPIOABASE+0x10)&(1<<8))==0) break;
    }
}
//------------------------------------------------------------------------
static void epd_reset ( unsigned int r )
{
    //PB2     RESET
    if(r)
    {
        PUT32(GPIOBBASE+0x18,1<<(2+BSRR_SET));
    }
    else
    {
        PUT32(GPIOBBASE+0x18,1<<(2+BSRR_RESET));
    }
}
//------------------------------------------------------------------------
static void epd_dc ( unsigned int d )
{
    //PB11    D/C
    if(d)
    {
        PUT32(GPIOBBASE+0x18,1<<(11+BSRR_SET));
    }
    else
    {
        PUT32(GPIOBBASE+0x18,1<<(11+BSRR_RESET));
    }
}
//------------------------------------------------------------------------
static void epd_cs ( unsigned int cs )
{
    //PA15    CS
    if(cs)
    {
        PUT32(GPIOABASE+0x18,1<<(15+BSRR_SET));
    }
    else
    {
        PUT32(GPIOABASE+0x18,1<<(15+BSRR_RESET));
    }
}
//------------------------------------------------------------------------
static void epd_sck ( unsigned int sck )
{
    //PB3     SCK
    if(sck)
    {
        PUT32(GPIOBBASE+0x18,1<<(3+BSRR_SET));
    }
    else
    {
        PUT32(GPIOBBASE+0x18,1<<(3+BSRR_RESET));
    }
}
//------------------------------------------------------------------------
static void epd_mosi ( unsigned int mosi )
{
    //PB5     MOSI
    if(mosi)
    {
        PUT32(GPIOBBASE+0x18,1<<(5+BSRR_SET));
    }
    else
    {
        PUT32(GPIOBBASE+0x18,1<<(5+BSRR_RESET));
    }
}
//------------------------------------------------------------------------
static void epd_command ( unsigned int reg )
{
    unsigned int ra;
    unsigned int rb;

    rb=reg;
    epd_cs(0);
    epd_dc(0);
    delay();
    for(ra=0;ra<8;ra++,rb<<=1)
    {
        epd_sck(0);
        epd_mosi((rb>>7)&1);
        delay();
        epd_sck(1);
        delay();
    }
    epd_cs(1);
}
//------------------------------------------------------------------------
static void epd_data ( unsigned int data )
{
    unsigned int ra;
    unsigned int rb;

    rb=data;
    epd_cs(0);
    epd_dc(1);
    delay();
    for(ra=0;ra<8;ra++,rb<<=1)
    {
        epd_sck(0);
        epd_mosi((rb>>7)&1);
        delay();
        epd_sck(1);
        delay();
    }
    epd_cs(1);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
unsigned int dome ( unsigned int x )
{
    unsigned int rb;

    rb=0;
    if(x&8) rb|=0xC0;
    if(x&4) rb|=0x30;
    if(x&2) rb|=0x0C;
    if(x&1) rb|=0x03;
    return(~rb);
}
//------------------------------------------------------------------------
unsigned int doxe ( unsigned int x )
{
    unsigned int rb;

    rb=0;
    if(x&2) rb|=0xF0;
    if(x&1) rb|=0x0F;
    return(~rb);
}
//------------------------------------------------------------------------
unsigned int doze ( unsigned int x )
{
    unsigned int rb;

    rb=0;
    if(x&1) rb=0xFF;
    return(~rb);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    clock_init();
    uart_init();
    hexstring(0x12345678);

    gpio_init();
    //epd_power(0);
    epd_cs(0);
    epd_cs(1);
    epd_sck(1);
    epd_mosi(0);
    epd_dc(0);
    epd_reset(0);
    for(ra=0;ra<10000;ra++) dummy(ra);
    epd_reset(1);
    for(ra=0;ra<10000;ra++) dummy(ra);
    epd_busy_wait();

    epd_command(0x10);
    epd_data(0x00);

    epd_command(0x11);
    epd_data(0x01);
    epd_command(0x44);
    epd_data(0x00);
    epd_data(0x11);
    epd_command(0x45);
    epd_data(0xAB);
    epd_data(0x00);
    epd_command(0x4E);
    epd_data(0x00);
    epd_command(0x4F);
    epd_data(0xAB);
    
    epd_command(0xF0);
    epd_data(0x1F);
    epd_command(0x2C);
    epd_data(0xA0);
    epd_command(0x3C);
    epd_data(0x63);
    epd_command(0x32);
    for(ra=0;ra<90;ra++)
    {
        epd_data(WF_LUT[ra]);
    }
    epd_busy_wait();

    epd_command(0x24);
    for(ra=0;ra<172;ra++)
    {
        for(rb=0;rb<18;rb++)
        {
            rc=0xFF;
            switch(rb)
            {
                case  0: rc=dome(fontdata['A'+(ra>>3)][ra&7]>>4); break;
                case  1: rc=dome(fontdata['A'+(ra>>3)][ra&7]>>0); break;

                case  2: rc=doxe(fontdata['A'+(ra>>4)][(ra>>1)&7]>>6); break;
                case  3: rc=doxe(fontdata['A'+(ra>>4)][(ra>>1)&7]>>4); break;
                case  4: rc=doxe(fontdata['A'+(ra>>4)][(ra>>1)&7]>>2); break;
                case  5: rc=doxe(fontdata['A'+(ra>>4)][(ra>>1)&7]>>0); break;
                
                case  6: rc=doze(fontdata['A'+(ra>>5)][(ra>>2)&7]>>7); break;
                case  7: rc=doze(fontdata['A'+(ra>>5)][(ra>>2)&7]>>6); break;
                case  8: rc=doze(fontdata['A'+(ra>>5)][(ra>>2)&7]>>5); break;
                case  9: rc=doze(fontdata['A'+(ra>>5)][(ra>>2)&7]>>4); break;
                case 10: rc=doze(fontdata['A'+(ra>>5)][(ra>>2)&7]>>3); break;
                case 11: rc=doze(fontdata['A'+(ra>>5)][(ra>>2)&7]>>2); break;
                case 12: rc=doze(fontdata['A'+(ra>>5)][(ra>>2)&7]>>1); break;
                case 13: rc=doze(fontdata['A'+(ra>>5)][(ra>>2)&7]>>0); break;
            }
            epd_data(rc);
        }
    }
    epd_command(0x21);
    epd_data(0x03);
    epd_command(0x22);
    epd_data(0xC4);
    epd_command(0x20);
    epd_busy_wait();


    hexstring(0x12341234);

    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
