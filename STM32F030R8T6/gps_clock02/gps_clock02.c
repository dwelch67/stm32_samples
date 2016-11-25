
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE     0x40021000
#define USART1BASE  0x40013800

#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400
#define GPIOCBASE 0x48000800
#define GPIODBASE 0x48000C00



static unsigned char xstring[32];
static unsigned int tim[4];
static unsigned int lasttim[4];
//#define ZMASK 0xFF
//static unsigned char zbuff[ZMASK+1];
//static unsigned int zhead;
//static unsigned int ztail;

static unsigned int stick;

#include "mask.h"


//------------------------------------------------------------------------
static void lcd_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    ra|=1<<18; //enable port b
    ra|=1<<19; //enable port c
    ra|=1<<20; //enable port d
    PUT32(RCCBASE+0x14,ra);

    PUT32(GPIOABASE+0x00,0x68015555);
    PUT32(GPIOABASE+0x04,0x00000000);
    PUT32(GPIOABASE+0x08,0xCC0003FF);
    PUT32(GPIOABASE+0x0C,0x24000000);

    PUT32(GPIOBBASE+0x00,0x55555555);
    PUT32(GPIOBBASE+0x04,0x00000000);
    PUT32(GPIOBBASE+0x08,0xFFFFFFFF);
    PUT32(GPIOBBASE+0x0C,0x00000000);

    PUT32(GPIOCBASE+0x00,0x55555555);
    PUT32(GPIOCBASE+0x04,0x00000000);
    PUT32(GPIOCBASE+0x08,0xFFFFFFFF);
    PUT32(GPIOCBASE+0x0C,0x00000000);

    //33221100
    //3210
    PUT32(GPIODBASE+0x00,0x00000010);
    PUT32(GPIODBASE+0x04,0x00000000);
    PUT32(GPIODBASE+0x08,0x00000030);
    PUT32(GPIODBASE+0x0C,0x00000000);


        PUT32(GPIOABASE+0x18,0x81FF0000);
        PUT32(GPIOBBASE+0x18,0xFFFF0000);
        PUT32(GPIOCBASE+0x18,0xFFFF0000);
        PUT32(GPIODBASE+0x18,0x00040000);

        //PUT32(GPIOABASE+0x18,0x000081FF);
        //PUT32(GPIOBBASE+0x18,0x0000FFFF);
        //PUT32(GPIOCBASE+0x18,0x0000FFFF);
        //PUT32(GPIODBASE+0x18,0x00000004);

    PUT32(GPIOABASE+0x18,(1<<7)<<0);

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

    //PUT32(USART1BASE+0x0C,833);
    PUT32(USART1BASE+0x0C,1667);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}
//------------------------------------------------------------------------
static void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
}
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
    //}
    while(1)
    {
        if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    }
    return(GET32(USART1BASE+0x24));
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------

//------------------------------------------------------------------------
static void show_clock ( void )
{
    unsigned int ra;

    if((stick++)&1) PUT32(GPIOABASE+0x18,(1<<7)<<0);
    else            PUT32(GPIOABASE+0x18,(1<<7)<<16);

    ra=0;
    if(tim[0]!=lasttim[0]) ra++;
    if(tim[1]!=lasttim[1]) ra++;
    if(tim[2]!=lasttim[2]) ra++;
    if(tim[3]!=lasttim[3]) ra++;
    if(ra==0) return;

    lasttim[0]=tim[0];
    lasttim[1]=tim[1];
    lasttim[2]=tim[2];
    lasttim[3]=tim[3];

    uart_send(0x30+lasttim[0]);
    uart_send(0x30+lasttim[1]);
    uart_send(0x30+lasttim[2]);
    uart_send(0x30+lasttim[3]);
    uart_send(0x0D);
    uart_send(0x0A);


    for(ra=0;ra<4;ra++)
    {
        PUT32(GPIOABASE+0x18,mask[ra][0][lasttim[ra]]);
        PUT32(GPIOBBASE+0x18,mask[ra][1][lasttim[ra]]);
        PUT32(GPIOCBASE+0x18,mask[ra][2][lasttim[ra]]);
        PUT32(GPIODBASE+0x18,mask[ra][3][lasttim[ra]]);
    }
    if(lasttim[0]==0)
    {
        PUT32(GPIOABASE+0x18,zero_off[0]);
        PUT32(GPIOBBASE+0x18,zero_off[1]);
        PUT32(GPIOCBASE+0x18,zero_off[2]);
        PUT32(GPIODBASE+0x18,zero_off[3]);
    }

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
                if(ra=='P') state++;
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
                        tim[0]=0;
                        tim[1]=0;
                        tim[2]=0;
                        tim[3]=0;
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
    lcd_init();
    uart_init();

//uart_send('U');

//{
    //unsigned int ra;
    //while(1)
    //{
        //PUT32(GPIOABASE+0x18,0x000081FF);
        //PUT32(GPIOBBASE+0x18,0x0000FFFF);
        //PUT32(GPIOCBASE+0x18,0x0000FFFF);
        //PUT32(GPIODBASE+0x18,0x00000004);
        //for(ra=0;ra<200000;ra++) dummy(ra);
        //PUT32(GPIOABASE+0x18,0x81FF0000);
        //PUT32(GPIOBBASE+0x18,0xFFFF0000);
        //PUT32(GPIOCBASE+0x18,0xFFFF0000);
        //PUT32(GPIODBASE+0x18,0x00040000);
        //for(ra=0;ra<200000;ra++) dummy(ra);
    //}
//}


//uart_send('X');
    stick=0;

    timezone=4;
    if(GET32(0x20000400)==0x12341234) timezone=5;
    PUT32(0x20000400,0x12341234);

    tim[0]=1;
    tim[1]=2;
    tim[2]=3;
    tim[3]=4;

    lasttim[0]=0;
    lasttim[1]=0;
    lasttim[2]=0;
    lasttim[3]=0;

    show_clock();

    //zhead=0;
    //ztail=0;

//uart_send('Y');

    do_nmea();

    return(0);
}
